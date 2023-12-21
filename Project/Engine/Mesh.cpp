#include "pch.h"

#include "Device.h"

#include "Mesh.h"
namespace ff7r
{
	Mesh::Mesh(bool _bEngine)
		: Resource(RES_TYPE::MESH, _bEngine)
		, vertex_buff{}
		, vertex_desc{}
		, vertex_count(0)
		, index_buff{}
		, index_desc{}
		, index_count(0)
		, vertex_sys_mem(nullptr)
		, index_sys_mem(nullptr)
	{
	}

	Mesh::~Mesh()
	{
		if (nullptr != vertex_sys_mem)
		{
			delete vertex_sys_mem;
		}

		if (nullptr != index_sys_mem)
		{
			delete index_sys_mem;
		}
	}

	void Mesh::Create(void* _vtx_sys_mem, UINT _vtx_cnt, void* _idx_sys_mem, UINT _idx_cnt)
	{
		vertex_count = _vtx_cnt;
		index_count = _idx_cnt;

		// SystemMem 데이터 복사
		vertex_sys_mem = new Vtx[vertex_count];
		memcpy(vertex_sys_mem, _vtx_sys_mem, sizeof(Vtx) * vertex_count);

		index_sys_mem = new UINT[index_count];
		memcpy(index_sys_mem, _idx_sys_mem, sizeof(UINT) * index_count);


		// Vertex 버퍼 생성
		vertex_desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		vertex_desc.CPUAccessFlags = 0;
		vertex_desc.Usage = D3D11_USAGE_DEFAULT;
		vertex_desc.ByteWidth = sizeof(Vtx) * vertex_count;

		D3D11_SUBRESOURCE_DATA tSub = {};
		tSub.pSysMem = _vtx_sys_mem;
		if (FAILED(DEVICE->CreateBuffer(&vertex_desc, &tSub, vertex_buff.GetAddressOf())))
		{
			assert(nullptr);
		}

		// Index 버퍼 생성
		index_desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
		index_desc.CPUAccessFlags = 0;
		index_desc.Usage = D3D11_USAGE_DEFAULT;
		index_desc.ByteWidth = sizeof(UINT) * index_count;

		tSub.pSysMem = _idx_sys_mem;
		if (FAILED(DEVICE->CreateBuffer(&index_desc, &tSub, index_buff.GetAddressOf())))
		{
			assert(nullptr);
		}
	}

	//void Mesh::CreateTempMesh(void* _VtxSysMem, UINT _iVtxCount, void* _IdxSysMem, UINT _IdxCount)
	//{
	//	m_VtxCount = _iVtxCount;
	//	m_IdxCount = _IdxCount;

	//	// SystemMem 데이터 복사
	//	m_pVtxSys = new CustomVertex[m_VtxCount];
	//	memcpy(m_pVtxSys, _VtxSysMem, sizeof(CustomVertex) * m_VtxCount);

	//	m_pIdxSys = new UINT[m_IdxCount];
	//	memcpy(m_pIdxSys, _IdxSysMem, sizeof(UINT) * m_IdxCount);


	//	// Vertex 버퍼 생성
	//	m_tVBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	//	m_tVBDesc.CPUAccessFlags = 0;
	//	m_tVBDesc.Usage = D3D11_USAGE_DEFAULT;
	//	m_tVBDesc.ByteWidth = sizeof(CustomVertex) * m_VtxCount;

	//	D3D11_SUBRESOURCE_DATA tSub = {};
	//	tSub.pSysMem = _VtxSysMem;
	//	if (FAILED(DEVICE->CreateBuffer(&m_tVBDesc, &tSub, m_VB.GetAddressOf())))
	//	{
	//		assert(nullptr);
	//	}

	//	// Index 버퍼 생성
	//	m_tIBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	//	m_tIBDesc.CPUAccessFlags = 0;
	//	m_tIBDesc.Usage = D3D11_USAGE_DEFAULT;
	//	m_tIBDesc.ByteWidth = sizeof(UINT) * m_IdxCount;

	//	tSub.pSysMem = _IdxSysMem;
	//	if (FAILED(DEVICE->CreateBuffer(&m_tIBDesc, &tSub, m_IB.GetAddressOf())))
	//	{
	//		assert(nullptr);
	//	}
	//}

	void Mesh::UpdateData()
	{
		UINT iStride = sizeof(Vtx);

		//if (i == 1) iStride = sizeof(CustomVertex);
		UINT iOffset = 0;

		CONTEXT->IASetVertexBuffers(0, 1, vertex_buff.GetAddressOf(), &iStride, &iOffset);
		CONTEXT->IASetIndexBuffer(index_buff.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	void Mesh::Render()
	{
		UpdateData();

		CONTEXT->DrawIndexed(index_count, 0, 0);
	}

	void Mesh::RenderParticle(UINT _iParticleCount)
	{
		UpdateData();

		// 인스턴싱
		CONTEXT->DrawIndexedInstanced(index_count, _iParticleCount, 0, 0, 0);
	}
}