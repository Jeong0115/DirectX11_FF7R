#pragma once
#include "ResUI.h"
namespace ff7r
{
    class SoundUI :
        public ResourceUI
    {

    public:
        virtual int RenderUpdate() override;

    public:
        SoundUI();
        ~SoundUI();
    };

}