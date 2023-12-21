#include "pch.h"

#include "Texture.h"

#include "SetColorShader.h"
namespace ff7r
{
	SetColorShader::SetColorShader(UINT _thread_x_per_group, UINT _thread_y_per_group, UINT _thread_z_per_group)
	{
		thread_x_per_group = _thread_x_per_group;
		thread_y_per_group = _thread_y_per_group;
		thread_z_per_group = _thread_z_per_group;
	}

	SetColorShader::~SetColorShader()
	{
	}

	void SetColorShader::UpdateData()
	{
		target_tex->UpdateDataCS(0, false);

		// 그룹 개수 계산
		group_x_cnt = target_tex->GetWidth() / thread_x_per_group;
		group_y_cnt = target_tex->GetHeight() / thread_y_per_group;
		group_z_cnt = 1;
	}

	void SetColorShader::Clear()
	{
		target_tex->Clear();
	}
}