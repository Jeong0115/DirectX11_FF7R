#pragma once

#include "ptr.h"
#include "Texture.h"


namespace ff7r
{
	class Texture;
	class ConstBuffer;
	class Device : public Singleton<Device>
	{
	public:
		Device();
		~Device();

		int		Init(HWND _hWnd, UINT _width, UINT _height);
		void	Present() { swapchain->Present(0, 0); }

		vec2	GetRenderResolution() { return render_resolution; }

		ID3D11Device* GetDevice() { return device.Get(); }
		ID3D11DeviceContext* GetDeviceContext() { return context.Get(); }

		ComPtr<ID3D11RasterizerState> GetRSState(RS_TYPE _type) { return rasterizer_state[(UINT)_type]; }
		ComPtr<ID3D11DepthStencilState> GetDSState(DS_TYPE _type) { return depthstencil_state[(UINT)_type]; }
		ComPtr<ID3D11BlendState> GetBSState(BS_TYPE _type) { return blend_state[(UINT)_type]; }

		ConstBuffer* GetConstBuffer(CB_TYPE _type) { return constbuffer[(UINT)_type]; }

	private:
		int		CreateSwapChain();
		int		CreateView();
		int		CreateRasterizerState();
		int		CreateBlendState();
		int		CreateDepthStencilState();
		int		CreateSampler();
		void	CreateConstBuffer();

		HWND							hWnd;

		ComPtr<ID3D11Device>			device;		
		ComPtr<ID3D11DeviceContext>		context;	
		ComPtr<IDXGISwapChain>			swapchain;
		ComPtr<ID3D11SamplerState>		sampler[2];
		ComPtr<ID3D11RasterizerState>	rasterizer_state[(UINT)RS_TYPE::END];
		ComPtr<ID3D11DepthStencilState>	depthstencil_state[(UINT)DS_TYPE::END];
		ComPtr<ID3D11BlendState>		blend_state[(UINT)BS_TYPE::END];

		D3D11_VIEWPORT					viewport;

		Ptr<Texture>					rendertarget_texture;
		Ptr<Texture>					depthstencil_texture;

		vec2							render_resolution;
		ConstBuffer*					constbuffer[(UINT)CB_TYPE::END];
	};
}

