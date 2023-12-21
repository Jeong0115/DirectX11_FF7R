#pragma once
#include "ComponentUI.h"

namespace ff7r
{
    class MeshRenderUI :
        public ComponentUI
    {

    public:
        virtual int RenderUpdate() override;


    public:
        void SelectMesh(DWORD_PTR _Key);
        void SelectMaterial(DWORD_PTR _Key);


    public:
        MeshRenderUI();
        ~MeshRenderUI();
    };

}