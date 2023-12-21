#pragma once
#include "UI.h"



#include <Engine\ptr.h>
#include <Engine\Resource.h>

namespace ff7r
{
    class ComponentUI;
    class ResourceUI;
    class AddComponentUI;
    class ScriptUI;
    class InspectorUI : public UI
    {
    public:
        InspectorUI();
        ~InspectorUI();

        virtual void Init() override;
        virtual void Tick() override;
        virtual void FinalTick() override;
        virtual int RenderUpdate() override;

        GameObject* GetTargetObject() { return target_obj; }
        void SetTargetObject(GameObject* _Target);
        void SetTargetResource(Ptr<Resource> _Res);

    private:
        void AddComponent(DWORD_PTR _Key);
        void AddComponentToTargetObject();
        void ClearTargetObject();
        void ClearTargetResource();

        ComponentUI*        components_ui[(UINT)COMPONENT_TYPE::END];
        ResourceUI*         resources_ui[(UINT)RES_TYPE::END];

        GameObject*         target_obj;
        Ptr<Resource>       target_res;

        AddComponentUI*     add_comp_ui;
        vector<ScriptUI*>   scripts_ui;
    };

}