#pragma once
#include "ComponentUI.h"
namespace ff7r
{
    class TileMapUI :
        public ComponentUI
    {

    public:
        virtual int RenderUpdate() override;

    public:
        TileMapUI();
        ~TileMapUI();
    };
}
