#pragma once

#include "ResUI.h"

namespace ff7r
{
    class MaterialUI : public ResourceUI
    {
    public:
        MaterialUI();
        ~MaterialUI();

        virtual int RenderUpdate() override;
        void SelectTexture(DWORD_PTR _Key);

    private:
        TEX_PARAM       selected_tex;
    };
}

