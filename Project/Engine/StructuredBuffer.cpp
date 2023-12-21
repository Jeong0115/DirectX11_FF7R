#include "pch.h"

#include "Device.h"

#include "StructuredBuffer.h"

namespace ff7r
{
	StructuredBuffer::StructuredBuffer()
		: element_size(0)
		, element_count(0)
	{
	}

	StructuredBuffer::~StructuredBuffer()
	{
	}

	void StructuredBuffer::Create(UINT _element_size, UINT _element_count, SB_TYPE _type
		, bool _use_sys_access, void* _sys_mem)
	{
		structed_buff = nullptr;
		shader_res_view = nullptr;
		unoredered_access_view = nullptr;

		read_buff = nullptr;
		write_buff = nullptr;

		type = _type;
		system_access = _use_sys_access;

		element_size = _element_size;
		element_count = _element_count;

		UINT buff_size = element_size * element_count;

		// 16바이트 단위 메모리 정렬
		assert(!(buff_size % 16));

		// 상수버퍼 생성
		desc.ByteWidth = buff_size;				// 버퍼 크기
		desc.StructureByteStride = element_size;	// 데이터 간격

		if (SB_TYPE::READ_ONLY == type)
		{
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	// Texture 레지스터에 바이딩하기 위한 플래그
		}
		else if (SB_TYPE::READ_WRITE == type)
		{
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		}

		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;	// 구조화 버퍼 체크
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;

		if (nullptr == _sys_mem)
		{
			if (FAILED(DEVICE->CreateBuffer(&desc, nullptr, structed_buff.GetAddressOf())))
			{
				assert(nullptr);
			}
		}
		else
		{
			D3D11_SUBRESOURCE_DATA sub_res_data = {};
			sub_res_data.pSysMem = _sys_mem;

			HRESULT hr = DEVICE->CreateBuffer(&desc, &sub_res_data, structed_buff.GetAddressOf());
			if (hr)
			{
				assert(nullptr);
			}
		}

		// ShaderResourceView 생성
		D3D11_SHADER_RESOURCE_VIEW_DESC	shader_res_desc = {};

		shader_res_desc.ViewDimension = D3D_SRV_DIMENSION_BUFFEREX;
		shader_res_desc.BufferEx.NumElements = element_count;

		if (FAILED(DEVICE->CreateShaderResourceView(structed_buff.Get(), &shader_res_desc, shader_res_view.GetAddressOf())))
		{
			assert(nullptr);
		}

		if (SB_TYPE::READ_WRITE == type)
		{

			D3D11_UNORDERED_ACCESS_VIEW_DESC unordered_access_desc = {};
			unordered_access_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			unordered_access_desc.Buffer.NumElements = element_count;

			if (FAILED(DEVICE->CreateUnorderedAccessView(structed_buff.Get(), &unordered_access_desc, unoredered_access_view.GetAddressOf())))
			{
				assert(nullptr);
			}
		}

		// CPU Access 보조 버퍼
		if (system_access)
		{
			desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;

			// GPU -> CPU Read
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			if (FAILED(DEVICE->CreateBuffer(&desc, nullptr, read_buff.GetAddressOf())))
			{
				assert(nullptr);
			}

			// CPU -> GPU Write
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			if (FAILED(DEVICE->CreateBuffer(&desc, nullptr, write_buff.GetAddressOf())))
			{
				assert(nullptr);
			}
		}
	}

	void StructuredBuffer::SetData(void* _src, UINT _size)
	{
		if (nullptr == _src)
			return;

		UINT size = _size;
		if (0 == size)
		{
			size = GetBufferSize();
		}

		// CPU -> CPU WriteBuffer
		D3D11_MAPPED_SUBRESOURCE sub_res = {};
		CONTEXT->Map(write_buff.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &sub_res);
		memcpy(sub_res.pData, _src, size);
		CONTEXT->Unmap(write_buff.Get(), 0);

		// CPU WriteBuffer -> Main Buffer
		CONTEXT->CopyResource(structed_buff.Get(), write_buff.Get());
	}

	void StructuredBuffer::GetData(void* _dst)
	{
		// Main Buffer -> CPU ReadBuffer
		CONTEXT->CopyResource(read_buff.Get(), structed_buff.Get());

		// CPU ReadBuffer -> CPU
		D3D11_MAPPED_SUBRESOURCE sub_res = {};
		CONTEXT->Map(read_buff.Get(), 0, D3D11_MAP::D3D11_MAP_READ, 0, &sub_res);
		memcpy(_dst, sub_res.pData, GetBufferSize());
		CONTEXT->Unmap(read_buff.Get(), 0);
	}

	void StructuredBuffer::UpdateData(UINT _register_num, UINT _stage)
	{
		register_number = _register_num;

		if (PIPELINE_STAGE::PS_VERTEX & _stage)
		{
			CONTEXT->VSSetShaderResources(register_number, 1, shader_res_view.GetAddressOf());
		}

		if (PIPELINE_STAGE::PS_HULL & _stage)
		{
			CONTEXT->HSSetShaderResources(register_number, 1, shader_res_view.GetAddressOf());
		}

		if (PIPELINE_STAGE::PS_DOMAIN & _stage)
		{
			CONTEXT->DSSetShaderResources(register_number, 1, shader_res_view.GetAddressOf());
		}

		if (PIPELINE_STAGE::PS_GEOMETRY & _stage)
		{
			CONTEXT->GSSetShaderResources(register_number, 1, shader_res_view.GetAddressOf());
		}

		if (PIPELINE_STAGE::PS_PIXEL & _stage)
		{
			CONTEXT->PSSetShaderResources(register_number, 1, shader_res_view.GetAddressOf());
		}
	}

	void StructuredBuffer::UpdateData_CS(UINT _register_num, bool _is_shader_res)
	{
		register_number = _register_num;

		if (_is_shader_res)
		{
			CONTEXT->CSSetShaderResources(register_number, 1, shader_res_view.GetAddressOf());
		}
		else
		{
			UINT i = -1;
			CONTEXT->CSSetUnorderedAccessViews(register_number, 1, unoredered_access_view.GetAddressOf(), &i);
		}
	}

	void StructuredBuffer::Clear()
	{
		ID3D11ShaderResourceView* pSRV = nullptr;
		CONTEXT->VSSetShaderResources(register_number, 1, &pSRV);
		CONTEXT->HSSetShaderResources(register_number, 1, &pSRV);
		CONTEXT->DSSetShaderResources(register_number, 1, &pSRV);
		CONTEXT->GSSetShaderResources(register_number, 1, &pSRV);
		CONTEXT->PSSetShaderResources(register_number, 1, &pSRV);
		CONTEXT->CSSetShaderResources(register_number, 1, &pSRV);

	}

	void StructuredBuffer::Clear_CS(bool _is_shader_res)
	{
		if (_is_shader_res)
		{
			ID3D11ShaderResourceView* pSRV = nullptr;
			CONTEXT->CSSetShaderResources(register_number, 1, &pSRV);
		}
		else
		{
			ID3D11UnorderedAccessView* pUAV = nullptr;
			UINT i = -1;
			CONTEXT->CSSetUnorderedAccessViews(register_number, 1, &pUAV, &i);
		}
	}
}