#pragma once
#include "ComponentUI.h"
namespace ff7r
{
    class Animator2DUI :
        public ComponentUI
    {

    public:
        virtual int RenderUpdate() override;

    public:
        Animator2DUI();
        ~Animator2DUI();
    };
}

