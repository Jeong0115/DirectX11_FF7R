#pragma once
#include "ResUI.h"
namespace ff7r
{
    class ComputeShaderUI :
        public ResourceUI
    {

    public:
        virtual int RenderUpdate() override;

    public:
        ComputeShaderUI();
        ~ComputeShaderUI();
    };
}

