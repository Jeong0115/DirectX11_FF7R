#include "pch.h"

#include "StructuredBuffer.h"

#include "ParticleShader.h"
namespace ff7r
{
	ParticleShader::ParticleShader(UINT _thread_x_per_group, UINT _thread_y_per_group, UINT _thread_z_per_group)
		: particle_buff(nullptr)
		, module_buff(nullptr)
		, rw_buff(nullptr)
	{
		thread_x_per_group = _thread_x_per_group;
		thread_y_per_group = _thread_y_per_group;
		thread_z_per_group = _thread_z_per_group;
	}

	ParticleShader::~ParticleShader()
	{
	}

	void ParticleShader::SetParticleBuffer(StructuredBuffer* _buff)
	{
		particle_buff = _buff;
		mtrl_const_data.array_int[0] = particle_buff->GetElementCount();
	}

	void ParticleShader::UpdateData()
	{
		particle_buff->UpdateData_CS(0, false);
		rw_buff->UpdateData_CS(1, false);
		module_buff->UpdateData_CS(20, true);
		noise_tex->UpdateDataCS(21, true);

		// ±×·ì ¼ö
		group_x_cnt = (particle_buff->GetElementCount() / thread_x_per_group) + 1;
	}

	void ParticleShader::Clear()
	{
		particle_buff->Clear_CS(false);
		rw_buff->Clear_CS(false);
		module_buff->Clear_CS(true);
		noise_tex->ClearCS(true);
	}
}