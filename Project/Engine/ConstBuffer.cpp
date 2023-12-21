#include "pch.h"

#include "Device.h"

#include "ConstBuffer.h"

namespace ff7r
{
	ConstBuffer::ConstBuffer(UINT _register_num)
		: desc{}
		, register_number(_register_num)
		, element_size(0)
		, element_count(0)
		, const_buff{}
	{
	}

	ConstBuffer::~ConstBuffer()
	{
	}

	void ConstBuffer::Create(UINT _element_size, UINT _element_count)
	{
		element_size = _element_size;
		element_count = _element_count;

		UINT buffer_size = _element_size * _element_count;

		// 16바이트 단위 메모리 정렬
		assert(!(buffer_size % 16));

		// 상수버퍼 생성
		desc.ByteWidth = buffer_size;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		if (FAILED(DEVICE->CreateBuffer(&desc, nullptr, const_buff.GetAddressOf())))
		{
			assert(nullptr);
		}
	}

	void ConstBuffer::SetData(void* _src, UINT _size)
	{
		// 크기가 지정되지 않은 데이터는 상수버퍼 크기로 본다.
		UINT size = _size;
		if (0 == _size)
		{
			size = element_size * element_count;
		}

		// 상수버퍼 크기보다 더 큰 데이터가 입력으로 들어온 경우
		assert(!(size > element_size * element_count));

		// SysMem -> GPU Mem
		D3D11_MAPPED_SUBRESOURCE sub_res = {};
		if (!FAILED(CONTEXT->Map(const_buff.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &sub_res)))
		{
			memcpy(sub_res.pData, _src, size);
			CONTEXT->Unmap(const_buff.Get(), 0);
		}
	}

	void ConstBuffer::UpdateData()
	{
		CONTEXT->VSSetConstantBuffers(register_number, 1, const_buff.GetAddressOf());
		CONTEXT->HSSetConstantBuffers(register_number, 1, const_buff.GetAddressOf());
		CONTEXT->DSSetConstantBuffers(register_number, 1, const_buff.GetAddressOf());
		CONTEXT->GSSetConstantBuffers(register_number, 1, const_buff.GetAddressOf());
		CONTEXT->PSSetConstantBuffers(register_number, 1, const_buff.GetAddressOf());
	}

	void ConstBuffer::UpdateData_CS()
	{
		CONTEXT->CSSetConstantBuffers(register_number, 1, const_buff.GetAddressOf());
	}
}
