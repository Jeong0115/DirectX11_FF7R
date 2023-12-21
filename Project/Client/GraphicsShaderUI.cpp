#include "pch.h"
#include "GraphicsShaderUI.h"

namespace ff7r
{
    GraphicsShaderUI::GraphicsShaderUI()
        : ResourceUI(RES_TYPE::GRAPHICS_SHADER)
    {
        SetName("GraphicsShader");
    }

    GraphicsShaderUI::~GraphicsShaderUI()
    {
    }

    int GraphicsShaderUI::RenderUpdate()
    {
        ResourceUI::RenderUpdate();

        return 0;
    }
}