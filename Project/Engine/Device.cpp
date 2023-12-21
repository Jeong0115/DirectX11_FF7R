#include "pch.h"

#include "ConstBuffer.h"
#include "GameEngine.h"
#include "ResourceMgr.h"
#include "Texture.h"

#include "Device.h"
namespace ff7r
{
    Device::Device()
        : hWnd(nullptr)
        , viewport{}
        , constbuffer{}
        , device(nullptr)
        , context(nullptr)
        , swapchain(nullptr)
        , sampler{}
        , rasterizer_state{}
        , depthstencil_state{}
        , blend_state{}
        , rendertarget_texture(nullptr)
        , depthstencil_texture(nullptr)
        , render_resolution{}
    {
    }

    Device::~Device()
    {
        Safe_Del_Array(constbuffer);
    }

    int Device::Init(HWND _hWnd, UINT _width, UINT _height)
    {
        hWnd = _hWnd;
        render_resolution = vec2((float)_width, (float)_height);

        GlobalData.resolution = render_resolution;

        int flag = 0;
#ifdef _DEBUG
        flag = D3D11_CREATE_DEVICE_DEBUG;
#endif

        D3D_FEATURE_LEVEL level = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

        if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE
            , nullptr, flag
            , nullptr, 0
            , D3D11_SDK_VERSION
            , device.GetAddressOf(), &level
            , context.GetAddressOf())))
        {
            MessageBox(nullptr, L"Device, Context 생성 실패", L"Device 초기화 에러", MB_OK);
            return E_FAIL;
        }

        if (FAILED(CreateSwapChain()))
        {
            MessageBox(nullptr, L"스왚체인 생성 실패", L"Device 초기화 에러", MB_OK);
            return E_FAIL;
        }

        if (FAILED(CreateView()))
        {
            MessageBox(nullptr, L"View 생성 실패", L"Device 초기화 에러", MB_OK);
            return E_FAIL;
        }

        viewport.TopLeftX = 0.f;
        viewport.TopLeftY = 0.f;

        vec2 window_resolution = ff7r::Engine::GetInst()->GetWindowResolution();
        viewport.Width = window_resolution.x;
        viewport.Height = window_resolution.y;

        viewport.MinDepth = 0.f;
        viewport.MaxDepth = 1.f;

        context->RSSetViewports(1, &viewport);


        if (FAILED(CreateRasterizerState()))
        {
            MessageBox(nullptr, L"레스터라이져 스테이트 생성 실패", L"Device 초기화 에러", MB_OK);
            return E_FAIL;
        }

        if (FAILED(CreateDepthStencilState()))
        {
            MessageBox(nullptr, L"DepthStencilState 생성 실패", L"Device 초기화 에러", MB_OK);
            return E_FAIL;
        }

        if (FAILED(CreateBlendState()))
        {
            MessageBox(nullptr, L"BlendState 생성 실패", L"Device 초기화 에러", MB_OK);
            return E_FAIL;
        }

        if (FAILED(CreateSampler()))
        {
            MessageBox(nullptr, L"샘플러 생성 실패", L"Device 초기화 에러", MB_OK);
            return E_FAIL;
        }

        CreateConstBuffer();

        return S_OK;
    }

    int Device::CreateSwapChain()
    {
        DXGI_SWAP_CHAIN_DESC desc = {};

        desc.OutputWindow = hWnd;    // 출력 윈도우
        desc.Windowed = true;          // 창모드, 전체화면 모드

        desc.BufferCount = 1; // *1에서 변경              
        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.BufferDesc.Width = (UINT)render_resolution.x;
        desc.BufferDesc.Height = (UINT)render_resolution.y;
        desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.BufferDesc.RefreshRate.Denominator = 1;
        desc.BufferDesc.RefreshRate.Numerator = 60;
        desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

        desc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD; // *DXGI_SWAP_EFFECT_DISCARD 에서 변경

        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Flags = 0;

        ComPtr<IDXGIDevice>  DXGI_device;
        ComPtr<IDXGIAdapter> adapter;
        ComPtr<IDXGIFactory> factory;

        HRESULT hr = S_OK;

        hr = device->QueryInterface(__uuidof(IDXGIDevice), (void**)DXGI_device.GetAddressOf());
        hr = DXGI_device->GetParent(__uuidof(IDXGIAdapter), (void**)adapter.GetAddressOf());
        hr = adapter->GetParent(__uuidof(IDXGIFactory), (void**)factory.GetAddressOf());
        hr = factory->CreateSwapChain(device.Get(), &desc, swapchain.GetAddressOf());

        return hr;
    }

    int Device::CreateView()
    {
        ComPtr<ID3D11Texture2D> tex2D;
        swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)tex2D.GetAddressOf());

        ResourceMgr::GetInst()->CreateTexture(L"RenderTargetTex", tex2D);

        ResourceMgr::GetInst()->CreateTexture(L"DepthStencilTex"
            , (UINT)render_resolution.x, (UINT)render_resolution.y
            , DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL, D3D11_USAGE_DEFAULT);

        return S_OK;
    }

    int Device::CreateRasterizerState()
    {
        rasterizer_state[(UINT)RS_TYPE::CULL_BACK] = nullptr;

        D3D11_RASTERIZER_DESC Desc = {};
        Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
        Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
        DEVICE->CreateRasterizerState(&Desc, rasterizer_state[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());

        Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
        Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
        DEVICE->CreateRasterizerState(&Desc, rasterizer_state[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());

        Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
        Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
        DEVICE->CreateRasterizerState(&Desc, rasterizer_state[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());

        return S_OK;
    }

    int Device::CreateDepthStencilState()
    {
        // Less
        depthstencil_state[(UINT)DS_TYPE::LESS] = nullptr;

        // Less Equal
        D3D11_DEPTH_STENCIL_DESC Desc = {};
        Desc.DepthEnable = true;
        Desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
        Desc.StencilEnable = false;
        Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        DEVICE->CreateDepthStencilState(&Desc, depthstencil_state[(UINT)DS_TYPE::LESS_EQUAL].GetAddressOf());

        // Greater
        Desc.DepthEnable = true;
        Desc.DepthFunc = D3D11_COMPARISON_GREATER;
        Desc.StencilEnable = false;
        Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        DEVICE->CreateDepthStencilState(&Desc, depthstencil_state[(UINT)DS_TYPE::GREATER].GetAddressOf());

        // Greater Equal
        Desc.DepthEnable = true;
        Desc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
        Desc.StencilEnable = false;
        Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        DEVICE->CreateDepthStencilState(&Desc, depthstencil_state[(UINT)DS_TYPE::GREATER_EQUAL].GetAddressOf());

        // No Write
        Desc.DepthEnable = true;
        Desc.DepthFunc = D3D11_COMPARISON_LESS;
        Desc.StencilEnable = false;
        Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        DEVICE->CreateDepthStencilState(&Desc, depthstencil_state[(UINT)DS_TYPE::NO_WRITE].GetAddressOf());

        // NoTest NoWrite
        Desc.DepthEnable = false;
        Desc.StencilEnable = false;
        Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        DEVICE->CreateDepthStencilState(&Desc, depthstencil_state[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf());


        return S_OK;
    }

    int Device::CreateBlendState()
    {
        // No Blend
        blend_state[(UINT)BS_TYPE::DEFAULT] = nullptr;

        // Mask
        D3D11_BLEND_DESC Desc = {};
        Desc.AlphaToCoverageEnable = true;
        Desc.IndependentBlendEnable = false;
        Desc.RenderTarget[0].BlendEnable = true;
        Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
        Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
        Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        DEVICE->CreateBlendState(&Desc, blend_state[(UINT)BS_TYPE::MASK].GetAddressOf());


        // Alpha Blend
        Desc.AlphaToCoverageEnable = false;
        Desc.IndependentBlendEnable = false;
        Desc.RenderTarget[0].BlendEnable = true;
        Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

        Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        DEVICE->CreateBlendState(&Desc, blend_state[(UINT)BS_TYPE::ALPHA_BLEND].GetAddressOf());

        // One One
        Desc.AlphaToCoverageEnable = false;
        Desc.IndependentBlendEnable = false;
        Desc.RenderTarget[0].BlendEnable = true;
        Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
        Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
        Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

        Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        DEVICE->CreateBlendState(&Desc, blend_state[(UINT)BS_TYPE::ONE_ONE].GetAddressOf());

        return S_OK;
    }

    int Device::CreateSampler()
    {
        D3D11_SAMPLER_DESC desc = {};

        desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.Filter = D3D11_FILTER_ANISOTROPIC;
        DEVICE->CreateSamplerState(&desc, sampler[0].GetAddressOf());

        desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        DEVICE->CreateSamplerState(&desc, sampler[1].GetAddressOf());

        CONTEXT->VSSetSamplers(0, 1, sampler[0].GetAddressOf());
        CONTEXT->HSSetSamplers(0, 1, sampler[0].GetAddressOf());
        CONTEXT->DSSetSamplers(0, 1, sampler[0].GetAddressOf());
        CONTEXT->GSSetSamplers(0, 1, sampler[0].GetAddressOf());    
        CONTEXT->PSSetSamplers(0, 1, sampler[0].GetAddressOf());
                                     
        CONTEXT->VSSetSamplers(1, 1, sampler[1].GetAddressOf());
        CONTEXT->HSSetSamplers(1, 1, sampler[1].GetAddressOf());
        CONTEXT->DSSetSamplers(1, 1, sampler[1].GetAddressOf());
        CONTEXT->GSSetSamplers(1, 1, sampler[1].GetAddressOf());
        CONTEXT->PSSetSamplers(1, 1, sampler[1].GetAddressOf());

        return S_OK;
    }

    void Device::CreateConstBuffer()
    {
        constbuffer[(UINT)CB_TYPE::TRANSFORM] = new ConstBuffer((UINT)CB_TYPE::TRANSFORM);
        constbuffer[(UINT)CB_TYPE::TRANSFORM]->Create(sizeof(TransformData), 1);

        constbuffer[(UINT)CB_TYPE::MATERIAL] = new ConstBuffer((UINT)CB_TYPE::MATERIAL);
        constbuffer[(UINT)CB_TYPE::MATERIAL]->Create(sizeof(MtrlConst), 1);

        constbuffer[(UINT)CB_TYPE::GLOBAL] = new ConstBuffer((UINT)CB_TYPE::GLOBAL);
        constbuffer[(UINT)CB_TYPE::GLOBAL]->Create(sizeof(Global), 1);
    }
}