#pragma once
#include "ComponentUI.h"

namespace ff7r
{
    class SkyBoxUI : public ComponentUI
    {
    public:
        SkyBoxUI();
        ~SkyBoxUI();

        virtual int RenderUpdate() override;

        void SelectMesh(DWORD_PTR _key);
    };
}