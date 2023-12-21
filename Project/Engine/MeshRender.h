#pragma once

#include "RenderComponent.h"

namespace ff7r
{
    class MeshRender : public RenderComponent
    {
    public:
        MeshRender();
        ~MeshRender();

        CLONE(MeshRender);

        virtual void FinalTick() override;
        virtual void Render() override;
    };
}
