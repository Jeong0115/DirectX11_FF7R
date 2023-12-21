#pragma once

#include "Resource.h"

namespace ff7r
{
	class Mesh : public Resource
	{
	public:
		Mesh(bool _is_engine_res = false);
		~Mesh();

		void Render();
		void RenderParticle(UINT _particle_cnt);
		void Create(void* _vtx_sys_mem, UINT _vtx_cnt, void* _idx_sys_mem, UINT _idx_cnt);

		virtual int Save(const wstring& _path) { return S_OK; }

	private:
		virtual int Load(const wstring& _path) { return S_OK; }
		virtual void UpdateData() override;

		ComPtr<ID3D11Buffer>	vertex_buff;
		D3D11_BUFFER_DESC		vertex_desc;
		UINT					vertex_count;

		ComPtr<ID3D11Buffer>	index_buff;
		D3D11_BUFFER_DESC		index_desc;
		UINT					index_count;

		void*					vertex_sys_mem;
		void*					index_sys_mem;
	};
}