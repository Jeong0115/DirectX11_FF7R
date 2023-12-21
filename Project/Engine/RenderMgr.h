#pragma once
#include "ptr.h"

#include "Texture.h"

namespace ff7r
{
    class Camera;
    class Light2D;
    class MRT;
    class StructuredBuffer;

    class RenderMgr : public Singleton<RenderMgr>
    {
        SINGLE(RenderMgr);
    public:
        void    Init();
        void    Render();

        int     RegisterCamera(Camera* _camera, int _idx);
        void    RegisterEditorCamera(Camera* _camera) { camera_editor = _camera; }
        void    SetRenderFunc(bool is_play);
        void    RegisterLight2D(const LightInfo& _light2d) { light2d.push_back(_light2d); }
        void    RegisterLight3D(const LightInfo& _light3d) { light3d.push_back(_light3d); }
        void    ClearCamera() { cameras.clear(); }
        void    CopyRenderTarget();

        void    AddDebugShapeInfo(const DebugShapeInfo& _info) { shape_info.push_back(_info); }
        vector<DebugShapeInfo>& GetDebugShapeInfo() { return shape_info; }

        Camera* GetMainCamera();
        MRT* GetMRT(MRT_TYPE _type) { return multiple_render_targets[(UINT)_type]; }
    private:
        void UpdateData();
        void RenderPlay();
        void RenderEditor();
        void Clear();
        void CreateMRT();
        void ClearMRT();

        void (RenderMgr::* RENDER_FUNC)(void);

        vector<Camera*>             cameras;       
        Camera*                     camera_editor; 
        vector<DebugShapeInfo>      shape_info;
        vector<LightInfo>           light2d;
        vector<LightInfo>           light3d;
        Ptr<Texture>                copy_rendertarget_tex;

        StructuredBuffer*           light2d_buff;
        StructuredBuffer*           light3d_buff;

        MRT*                        multiple_render_targets[(UINT)MRT_TYPE::END];
    };
}

