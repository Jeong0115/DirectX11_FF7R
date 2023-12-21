#pragma once
#include "UI.h"

#include <Engine\ptr.h>
#include <Engine\Resource.h>

namespace ff7r
{
    class ComponentUI :
        public UI
    {
    private:
        GameObject* m_Target;
        const COMPONENT_TYPE    m_Type;

    public:
        void SetTarget(GameObject* _Target);

        GameObject* GetTarget() { return m_Target; }

        COMPONENT_TYPE GetComponentType() { return m_Type; }

        void GetResKey(Ptr<Resource> _Res, char* _Buff, size_t _BufferSize);

    public:
        virtual int RenderUpdate() override;


    public:
        ComponentUI(const string& _Name, COMPONENT_TYPE _Type);
        ~ComponentUI();
    };
}
