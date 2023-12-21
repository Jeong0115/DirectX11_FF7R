#include "pch.h"
#include "ComputeShaderUI.h"
namespace ff7r
{
    ComputeShaderUI::ComputeShaderUI()
        : ResourceUI(RES_TYPE::COMPUTE_SHADER)
    {
        SetName("ComputeShader");
    }

    ComputeShaderUI::~ComputeShaderUI()
    {
    }

    int ComputeShaderUI::RenderUpdate()
    {
        ResourceUI::RenderUpdate();

        return 0;
    }
}