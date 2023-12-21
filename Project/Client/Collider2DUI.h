#pragma once
#include "ComponentUI.h"
namespace ff7r
{
    class Collider2DUI :
        public ComponentUI
    {
    public:
        virtual int RenderUpdate() override;

    public:
        Collider2DUI();
        ~Collider2DUI();
    };

}