#include "pch.h"
#include "RenderMgr.h"

#include "Device.h"
#include "ConstBuffer.h"
#include "StructuredBuffer.h"

#include "Camera.h"
#include "Light2D.h"
#include "MRT.h"

#include "ResourceMgr.h"

namespace ff7r
{
    RenderMgr::RenderMgr()
        : light2d_buff(nullptr)
        , light3d_buff(nullptr)
        , RENDER_FUNC(nullptr)
        , camera_editor(nullptr)
        , cameras{} 
        , shape_info{}
        , light2d{}
        , multiple_render_targets{}
    {
        vec2 resolution = Device::GetInst()->GetRenderResolution();
        copy_rendertarget_tex = ResourceMgr::GetInst()->CreateTexture(L"RTCopyTex", (UINT)resolution.x, (UINT)resolution.y
            , DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT);
    }

    RenderMgr::~RenderMgr()
    {
        if (nullptr != light2d_buff)
        {
            delete light2d_buff;
        }
        if (nullptr != light3d_buff)
        {
            delete light3d_buff;
        }

        Safe_Del_Array(multiple_render_targets);
    }


    void RenderMgr::Init()
    {
        // Light2DBuffer 구조화 버퍼 생성
        light2d_buff = new StructuredBuffer;
        light2d_buff->Create(sizeof(LightInfo), 10, SB_TYPE::READ_ONLY, true);

        light3d_buff = new StructuredBuffer;
        light3d_buff->Create(sizeof(LightInfo), 10, SB_TYPE::READ_ONLY, true);

        CreateMRT();
    }

    void RenderMgr::Render()
    {
        UpdateData();

        (this->*RENDER_FUNC)();

        Clear();
    }


    void RenderMgr::RenderPlay()
    {
        // 카메라 기준 렌더링
        for (size_t i = 0; i < cameras.size(); ++i)
        {
            if (nullptr == cameras[i])
                continue;

            // 물체 분류작업
            // - 해당 카메라가 볼 수 있는 물체(레이어 분류)
            // - 재질에 따른 분류 (재질->쉐이더) 쉐이더 도메인
            //   쉐이더 도메인에 따라서 렌더링 순서분류
            cameras[i]->SortObject();
            cameras[i]->Render();
        }
    }

    void RenderMgr::RenderEditor()
    {
        ClearMRT();

        camera_editor->SortObject();
        //multiple_render_targets[(UINT)MRT_TYPE::SWAPCHAIN]->OMSetRenderTargets();
        camera_editor->Render();
    }


    int RenderMgr::RegisterCamera(Camera* _camera, int _idx)
    {
        if (cameras.size() <= _idx)
        {
            cameras.resize(_idx + 1);
        }

        cameras[_idx] = _camera;
        return _idx;
    }

    void RenderMgr::SetRenderFunc(bool is_play)
    {
        if (is_play)
            RENDER_FUNC = &RenderMgr::RenderPlay;
        else
            RENDER_FUNC = &RenderMgr::RenderEditor;
    }

    void RenderMgr::CopyRenderTarget()
    {
        Ptr<Texture> rendertarget_tex = ResourceMgr::GetInst()->FindRes<Texture>(L"RenderTargetTex");
        CONTEXT->CopyResource(copy_rendertarget_tex->GetTex2D().Get(), rendertarget_tex->GetTex2D().Get());
    }

    Camera* RenderMgr::GetMainCamera()
    {
        if (cameras.empty())
            return nullptr;

        return cameras[0];
    }

    void RenderMgr::UpdateData()
    {
        // 구조화버퍼의 크기가 모자라면 더 크게 새로 만든다.
        if (light2d_buff->GetElementCount() < light2d.size())
        {
            light2d_buff->Create(sizeof(LightInfo), light2d.size(), SB_TYPE::READ_ONLY, true);
        }

        // 구조화버퍼로 광원 데이터를 옮긴다.
        light2d_buff->SetData(light2d.data(), sizeof(LightInfo) * light2d.size());
        light2d_buff->UpdateData(12, PIPELINE_STAGE::PS_PIXEL);

        if (light3d_buff->GetElementCount() < light3d.size())
        {
            light3d_buff->Create(sizeof(LightInfo), light3d.size(), SB_TYPE::READ_ONLY, true);
        }

        light3d_buff->SetData(light3d.data(), sizeof(LightInfo) * light3d.size());
        light3d_buff->UpdateData(13, PIPELINE_STAGE::PS_PIXEL);

        GlobalData.light2d_count = light2d.size();
        GlobalData.light3d_count = light3d.size();

        // 전역 상수 데이터 바인딩
        ConstBuffer* global_buff = Device::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
        global_buff->SetData(&GlobalData, sizeof(Global));
        global_buff->UpdateData();
        global_buff->UpdateData_CS();
    }

    void RenderMgr::Clear()
    {
        light2d.clear();
        light3d.clear();
    }

    void RenderMgr::CreateMRT()
    {
        multiple_render_targets[(UINT)MRT_TYPE::SWAPCHAIN] = new MRT;

        Ptr<Texture> textures[8] = {};
        textures[0] = ResourceMgr::GetInst()->FindRes<Texture>(L"RenderTargetTex");

        Ptr<Texture> depth_tex = ResourceMgr::GetInst()->FindRes<Texture>(L"DepthStencilTex");

        multiple_render_targets[(UINT)MRT_TYPE::SWAPCHAIN]->Create(textures, 1, depth_tex);
        multiple_render_targets[(UINT)MRT_TYPE::SWAPCHAIN]->SetClearColor(vec4(1.f, 0.f, 0.f, 1.f), 0);



        multiple_render_targets[(UINT)MRT_TYPE::DEFERRED] = new MRT;

        vec2 resolution = Device::GetInst()->GetRenderResolution();

        Ptr<Texture> deferred_textures[8] = {};
        deferred_textures[0] = ResourceMgr::GetInst()->CreateTexture(L"DiffuseTargetTex", resolution.x, resolution.y
            , DXGI_FORMAT_R8G8B8A8_UNORM
            , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        deferred_textures[1] = ResourceMgr::GetInst()->CreateTexture(L"NormalTargetTex", resolution.x, resolution.y
            , DXGI_FORMAT_R32G32B32A32_FLOAT
            , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        deferred_textures[2] = ResourceMgr::GetInst()->CreateTexture(L"PositionTargetTex", resolution.x, resolution.y
            , DXGI_FORMAT_R32G32B32A32_FLOAT
            , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        deferred_textures[3] = ResourceMgr::GetInst()->CreateTexture(L"DataTargetTex", resolution.x, resolution.y
            , DXGI_FORMAT_R32G32B32A32_FLOAT
            , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        multiple_render_targets[(UINT)MRT_TYPE::DEFERRED]->Create(deferred_textures, 4, nullptr);
    
    }

    void RenderMgr::ClearMRT()
    {
        for (UINT i = 0; i < (UINT)MRT_TYPE::END; ++i)
        {
            if (multiple_render_targets[i])
            {
                multiple_render_targets[i]->ClearTarget();
            }
        }
    }
}
