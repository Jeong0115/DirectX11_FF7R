#pragma once
#include "ResUI.h"
namespace ff7r
{
    class MeshDataUI :
        public ResourceUI
    {

    public:
        virtual int RenderUpdate() override;

    public:
        MeshDataUI();
        ~MeshDataUI();
    };

}