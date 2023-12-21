#include "pch.h"

#include "ConstBuffer.h"
#include "Device.h"
#include "PathMgr.h"

#include "ComputeShader.h"
namespace ff7r
{
	ComputeShader::ComputeShader()
		: Shader(RES_TYPE::COMPUTE_SHADER)
		, group_x_cnt(1)
		, group_y_cnt(1)
		, group_z_cnt(1)
		, mtrl_const_data{}
		, thread_x_per_group(1)
		, thread_y_per_group(1)
		, thread_z_per_group(1)
		, compute_shader{}
		, compute_shader_blob{}
	{
	}

	ComputeShader::~ComputeShader()
	{
	}

	void ComputeShader::CreateComputeShader(const wstring& _file_name, const string& _func_name)
	{
		// Shader 파일 경로
		wstring shader_file_name = PathMgr::GetInst()->GetContentPath();
		shader_file_name += _file_name;

		// Shader Compile	
		if (FAILED(D3DCompileFromFile(shader_file_name.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, _func_name.c_str(), "cs_5_0", 0, 0, compute_shader_blob.GetAddressOf(), error_blob.GetAddressOf())))
		{
			MessageBoxA(nullptr, (const char*)error_blob->GetBufferPointer()
				, "Compute Shader Compile Failed!!", MB_OK);
		}

		// 컴파일된 객체로 Shader 를 만든다.
		DEVICE->CreateComputeShader(compute_shader_blob->GetBufferPointer(), compute_shader_blob->GetBufferSize()
			, nullptr, compute_shader.GetAddressOf());
	}

	void ComputeShader::Execute()
	{
		UpdateData();

		static ConstBuffer* const_buff = Device::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
		const_buff->SetData(&mtrl_const_data);
		const_buff->UpdateData_CS();

		CONTEXT->CSSetShader(compute_shader.Get(), nullptr, 0);
		CONTEXT->Dispatch(group_x_cnt, group_y_cnt, group_z_cnt);

		Clear();
	}
}