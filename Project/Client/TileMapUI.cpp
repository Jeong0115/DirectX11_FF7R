#include "pch.h"
#include "TileMapUI.h"

namespace ff7r
{
    TileMapUI::TileMapUI()
        : ComponentUI("##TileMap", COMPONENT_TYPE::TILEMAP)
    {
        SetName("TileMap");
    }

    TileMapUI::~TileMapUI()
    {
    }

    int TileMapUI::RenderUpdate()
    {
        if (FALSE == ComponentUI::RenderUpdate())
            return FALSE;




        return TRUE;
    }
}