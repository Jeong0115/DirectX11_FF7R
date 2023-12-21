#include "pch.h"
#include "MeshDataUI.h"

namespace ff7r
{
    MeshDataUI::MeshDataUI()
        : ResourceUI(RES_TYPE::MESHDATA)
    {
        SetName("MeshData");
    }

    MeshDataUI::~MeshDataUI()
    {
    }


    int MeshDataUI::RenderUpdate()
    {
        ResourceUI::RenderUpdate();

        return 0;
    }
}