#pragma once
#include "ResUI.h"
namespace ff7r
{
    class GraphicsShaderUI :
        public ResourceUI
    {

    public:
        virtual int RenderUpdate() override;

    public:
        GraphicsShaderUI();
        ~GraphicsShaderUI();
    };
}
