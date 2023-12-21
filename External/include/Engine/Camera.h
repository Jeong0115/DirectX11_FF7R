#pragma once
#include "Component.h"

namespace ff7r
{
    class Camera : public Component
    {
    public:
        Camera();
        Camera(const Camera& _other);
        virtual ~Camera();

        void                    SetProjType(PROJ_TYPE _type) { proj_type = _type; }
        PROJ_TYPE               GetProjType() { return proj_type; }
        void                    SetScale(float _scale) { scale = _scale; }
        float                   GetScale() { return scale; }
        void                    SetLayerMask(int _layer, bool _visible);
        void                    SetLayerMaskAll(bool _visible);
        void                    SetCameraIndex(int _idx);

        const Matrix&           GetViewMat() { return mat_view; }
        const Matrix&           GetProjMat() { return mat_proj; }

        void                    SortObject();
        void                    Render();

        virtual void            Begin() override;
        virtual void            FinalTick() override;

    private:
        CLONE(Camera);

        void                    Clear();
        void                    RenderDeferred();
        void                    RenderOpaque();
        void                    RenderMask();
        void                    RenderTransparent();
        void                    RenderPostprocess();
        void                    RenderUI();
        void                    CalcViewMat();
        void                    CalcProjMat();

        virtual void            SaveToLevelFile(FILE* _file) override;
        virtual void            LoadFromLevelFile(FILE* _file) override;

        PROJ_TYPE               proj_type;

        Matrix                  mat_view;
        Matrix                  mat_proj;

        UINT                    layer_mask;
        float                   aspect_ratio;
        float                   scale;
        int                     camera_idx;          // 카메라 우선순위

        vector<GameObject*>     opaque_obj;
        vector<GameObject*>     mask_obj;
        vector<GameObject*>     transparent_obj;
        vector<GameObject*>     ui_obj;
        vector<GameObject*>     post_obj;

        vector<GameObject*>     deferred_obj;
    };

}