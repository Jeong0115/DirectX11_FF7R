#include "pch.h"
#include "Texture.h"

#include "Device.h"

namespace ff7r
{
	Texture::Texture(bool _bEngine)
		: Resource(RES_TYPE::TEXTURE, _bEngine)
		, desc{}
		, texture2d{}
		, shader_resource_view{}
		, render_target_view{}
		, depth_stencil_veiw{}
		, unordered_access_view{}
		, image{}
		, register_number(1)
	{
	}

	Texture::~Texture()
	{
	}

	void Texture::UpdateData(int _register_number, int _stage)
	{
		register_number = _register_number;

		if (PIPELINE_STAGE::PS_VERTEX & _stage)
		{
			CONTEXT->VSSetShaderResources(register_number, 1, shader_resource_view.GetAddressOf());
		}

		if (PIPELINE_STAGE::PS_HULL & _stage)
		{
			CONTEXT->HSSetShaderResources(register_number, 1, shader_resource_view.GetAddressOf());
		}

		if (PIPELINE_STAGE::PS_DOMAIN & _stage)
		{
			CONTEXT->DSSetShaderResources(register_number, 1, shader_resource_view.GetAddressOf());
		}

		if (PIPELINE_STAGE::PS_GEOMETRY & _stage)
		{
			CONTEXT->GSSetShaderResources(register_number, 1, shader_resource_view.GetAddressOf());
		}

		if (PIPELINE_STAGE::PS_PIXEL & _stage)
		{
			CONTEXT->PSSetShaderResources(register_number, 1, shader_resource_view.GetAddressOf());
		}
	}

	void Texture::UpdateDataCS(int _register_number, bool _is_shader_res)
	{
		register_number = _register_number;

		if (_is_shader_res)
		{
			CONTEXT->CSSetShaderResources(register_number, 1, shader_resource_view.GetAddressOf());
		}
		else
		{
			UINT i = -1;
			CONTEXT->CSSetUnorderedAccessViews(register_number, 1, unordered_access_view.GetAddressOf(), &i);
		}
	}

	void Texture::Clear()
	{
		ID3D11ShaderResourceView* srv = nullptr;
		CONTEXT->VSSetShaderResources(register_number, 1, &srv);
		CONTEXT->HSSetShaderResources(register_number, 1, &srv);
		CONTEXT->DSSetShaderResources(register_number, 1, &srv);
		CONTEXT->GSSetShaderResources(register_number, 1, &srv);
		CONTEXT->PSSetShaderResources(register_number, 1, &srv);
	}

	void Texture::Clear(int _register_number)
	{
		ID3D11ShaderResourceView* srv = nullptr;
		CONTEXT->VSSetShaderResources(_register_number, 1, &srv);
		CONTEXT->HSSetShaderResources(_register_number, 1, &srv);
		CONTEXT->DSSetShaderResources(_register_number, 1, &srv);
		CONTEXT->GSSetShaderResources(_register_number, 1, &srv);
		CONTEXT->PSSetShaderResources(_register_number, 1, &srv);
	}

	void Texture::ClearCS(bool _is_shader_res)
	{
		if (_is_shader_res)
		{
			ID3D11ShaderResourceView* srv = nullptr;
			CONTEXT->CSSetShaderResources(register_number, 1, &srv);
		}
		else
		{
			ID3D11UnorderedAccessView* uav = nullptr;
			UINT i = -1;
			CONTEXT->CSSetUnorderedAccessViews(register_number, 1, &uav, &i);
		}
	}

	void Texture::UpdateData()
	{
	}


	int Texture::Load(const wstring& _path)
	{
		wchar_t szExt[50] = L"";
		_wsplitpath_s(_path.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 50);
		wstring strExt = szExt;

		HRESULT hr = S_OK;
		if (L".dds" == strExt || L".DDS" == strExt)
		{
			// dds, DDS
			hr = LoadFromDDSFile(_path.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, image);
		}

		else if (L".tga" == strExt || L".TGA" == strExt)
		{
			// tga, TGA
			hr = LoadFromTGAFile(_path.c_str(), nullptr, image);
		}

		else
		{
			// png, jpg, jpeg, bmp
			hr = LoadFromWICFile(_path.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, image);
		}

		if (FAILED(hr))
		{
			MessageBox(nullptr, L"리소스 로딩 실패", L"텍스쳐 로딩 실패", MB_OK);
			return E_FAIL;
		}

		hr = CreateShaderResourceView(DEVICE
			, image.GetImages()
			, image.GetImageCount()
			, image.GetMetadata()
			, shader_resource_view.GetAddressOf());

		if (FAILED(hr))
		{
			MessageBox(nullptr, L"ShaderResourceView 생성 실패", L"텍스쳐 로딩 실패", MB_OK);
			return E_FAIL;
		}

		shader_resource_view->GetResource((ID3D11Resource**)texture2d.GetAddressOf());
		texture2d->GetDesc(&desc);

		return S_OK;
	}

	int Texture::Create(UINT _width, UINT _height, DXGI_FORMAT _pixel_format, UINT _bind_flag, D3D11_USAGE _usage)
	{
		// ID3D11Texture2D 생성
		desc.Format = _pixel_format;

		// 반드시 렌더타겟과 같은 해상도로 설정해야 함
		desc.Width = _width;
		desc.Height = _height;
		desc.ArraySize = 1;

		desc.BindFlags = _bind_flag;
		desc.Usage = _usage;

		if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == _usage)
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		else if (D3D11_USAGE::D3D11_USAGE_STAGING == _usage)
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

		desc.MipLevels = 1;    // 원본만 생성
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;


		if (FAILED(DEVICE->CreateTexture2D(&desc, nullptr, texture2d.GetAddressOf())))
		{
			return E_FAIL;
		}

		// 바인드 플래그에 맞는 View 를 생성해준다.
		if (desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
		{
			if (FAILED(DEVICE->CreateDepthStencilView(texture2d.Get(), nullptr, depth_stencil_veiw.GetAddressOf())))
			{
				return E_FAIL;
			}
		}
		else
		{
			if (desc.BindFlags & D3D11_BIND_RENDER_TARGET)
			{
				if (FAILED(DEVICE->CreateRenderTargetView(texture2d.Get(), nullptr, render_target_view.GetAddressOf())))
				{
					return E_FAIL;
				}
			}

			if (desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
			{
				if (FAILED(DEVICE->CreateShaderResourceView(texture2d.Get(), nullptr, shader_resource_view.GetAddressOf())))
				{
					return E_FAIL;
				}
			}

			if (desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
			{
				if (FAILED(DEVICE->CreateUnorderedAccessView(texture2d.Get(), nullptr, unordered_access_view.GetAddressOf())))
				{
					return E_FAIL;
				}
			}
		}


		return S_OK;
	}

	int Texture::Create(ComPtr<ID3D11Texture2D> _tex2D)
	{
		texture2d = _tex2D;

		texture2d->GetDesc(&desc);

		// 바인드 플래그에 맞는 View 를 생성해준다.
		if (desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
		{
			if (FAILED(DEVICE->CreateDepthStencilView(texture2d.Get(), nullptr, depth_stencil_veiw.GetAddressOf())))
			{
				return E_FAIL;
			}
		}
		else
		{
			if (desc.BindFlags & D3D11_BIND_RENDER_TARGET)
			{
				if (FAILED(DEVICE->CreateRenderTargetView(texture2d.Get(), nullptr, render_target_view.GetAddressOf())))
				{
					return E_FAIL;
				}
			}

			if (desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
			{
				if (FAILED(DEVICE->CreateShaderResourceView(texture2d.Get(), nullptr, shader_resource_view.GetAddressOf())))
				{
					return E_FAIL;
				}
			}

			if (desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
			{
				if (FAILED(DEVICE->CreateUnorderedAccessView(texture2d.Get(), nullptr, unordered_access_view.GetAddressOf())))
				{
					return E_FAIL;
				}
			}
		}

		return S_OK;
	}

	int Texture::Save(const wstring& _path)
	{
		return S_OK;
	}
}