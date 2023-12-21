#pragma once

#include "ResUI.h"
namespace ff7r
{
    class LevelUI
        : public ResourceUI
    {
    public:
        virtual int RenderUpdate() override;

    public:
        LevelUI();
        ~LevelUI();
    };

}