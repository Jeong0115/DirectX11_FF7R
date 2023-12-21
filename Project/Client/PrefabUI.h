#pragma once
#include "ResUI.h"
namespace ff7r
{
    class PrefabUI :
        public ResourceUI
    {

    public:
        virtual int RenderUpdate() override;

    public:
        PrefabUI();
        ~PrefabUI();
    };

}