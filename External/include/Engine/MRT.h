#pragma once

#include "Entity.h"
#include "Texture.h"

#include "ptr.h"

namespace ff7r
{
	class MRT : public Entity
	{
    public:
        MRT();
        ~MRT();

        CLONE_DISABLE(MRT);

        void Create(Ptr<Texture>* _targets, UINT _cnt, Ptr<Texture> _depth_tex);
        void ClearTarget();
        void OMSetRenderTargets(bool _preserve_depth_stencil_view = false);
        void SetClearColor(vec4 _color, UINT _idx) { clear_color[_idx] = _color; }

    private:
        Ptr<Texture>    render_targets[8];
        Ptr<Texture>    depth_stencil_tex;
        UINT            render_target_cnt;

        vec4            clear_color[8];

        D3D11_VIEWPORT  viewport;
	};
}

