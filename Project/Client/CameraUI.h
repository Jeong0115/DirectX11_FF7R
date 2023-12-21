#pragma once
#include "ComponentUI.h"
namespace ff7r
{
    class CameraUI :
        public ComponentUI
    {

    public:
        virtual int RenderUpdate() override;

    public:
        CameraUI();
        ~CameraUI();
    };

}