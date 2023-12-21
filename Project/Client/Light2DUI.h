#pragma once
#include "ComponentUI.h"
namespace ff7r
{
    class Light2DUI :
        public ComponentUI
    {

    public:
        virtual int RenderUpdate() override;

    public:
        Light2DUI();
        ~Light2DUI();
    };
}
