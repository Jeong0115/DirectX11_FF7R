#include "pch.h"

#include "Device.h"

#include "MRT.h"
namespace ff7r
{
	MRT::MRT()
		: viewport{}
		, render_target_cnt(0)
		, render_targets{}
		, depth_stencil_tex{}
	{
	}
	MRT::~MRT()
	{
	}

	void MRT::Create(Ptr<Texture>* _targets, UINT _cnt, Ptr<Texture> _depth_tex)
	{
		for (UINT i = 0; i < _cnt; i++)
		{
			render_targets[i] = _targets[i];
		}

		render_target_cnt = _cnt;
		depth_stencil_tex = _depth_tex;
	}

	void MRT::ClearTarget()
	{
		for (UINT i = 0; i < render_target_cnt; i++)
		{
			CONTEXT->ClearRenderTargetView(render_targets[i]->GetRTV().Get(), clear_color[i]);
		}

		if (nullptr != depth_stencil_tex)
		{
			CONTEXT->ClearDepthStencilView(depth_stencil_tex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0.f);
		}
	}

	void MRT::OMSetRenderTargets(bool _preserve_depth_stencil_view)
	{
		ID3D11RenderTargetView* render_target_views[8] = {};
		for (UINT i = 0; i < render_target_cnt; ++i)
		{
			render_target_views[i] = render_targets[i]->GetRTV().Get();
		}

		if (depth_stencil_tex != nullptr)
		{
			CONTEXT->OMSetRenderTargets(render_target_cnt, render_target_views, depth_stencil_tex->GetDSV().Get());
		}
		else
		{
			ComPtr<ID3D11DepthStencilView> depth_stencil_view = nullptr;

			if (_preserve_depth_stencil_view)
			{
				CONTEXT->OMGetRenderTargets(0, nullptr, depth_stencil_view.GetAddressOf());
			}

			CONTEXT->OMSetRenderTargets(render_target_cnt, render_target_views, depth_stencil_view.Get());
		}
	}
}