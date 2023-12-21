#include "pch.h"

#include "Device.h"
#include "ConstBuffer.h"

#include "Transform.h"
namespace ff7r
{
	Transform::Transform()
		: Component(COMPONENT_TYPE::TRANSFORM)
		, relative_pos(vec3(0.f, 0.f, 0.f))
		, relative_scale(vec3(1.f, 1.f, 1.f))
		, relative_rotation(vec3(0.f, 0.f, 0.f))
		, relative_dir{ vec3(1.f, 0.f, 0.f), vec3(0.f, 1.f, 0.f), vec3(0.f, 0.f, 1.f) }
		, world_dir{ vec3(0.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f) }
		, mat_world{}
		, mat_world_scale{}
		, absolute(false)
	{
		SetName(L"Transform");
	}

	Transform::~Transform()
	{
	}

	void Transform::FinalTick()
	{
		mat_world_scale = XMMatrixIdentity();
		mat_world_scale = XMMatrixScaling(relative_scale.x, relative_scale.y, relative_scale.z);

		Matrix mat_rot = XMMatrixIdentity();
		mat_rot = XMMatrixRotationX(relative_rotation.x);
		mat_rot *= XMMatrixRotationY(relative_rotation.y);
		mat_rot *= XMMatrixRotationZ(relative_rotation.z);

		Matrix mat_translation = XMMatrixTranslation(relative_pos.x, relative_pos.y, relative_pos.z);
		mat_world = mat_world_scale * mat_rot * mat_translation;

		vec3 default_dir[3] = { vec3(1.f, 0.f, 0.f), vec3(0.f, 1.f, 0.f), vec3(0.f, 0.f, 1.f) };

		for (int i = 0; i < 3; ++i)
		{
			world_dir[i] = relative_dir[i] = XMVector3TransformNormal(default_dir[i], mat_rot);
		}

		// 부모 오브젝트 확인
		GameObject* parent = GetOwner()->GetParent();
		if (parent)
		{
			if (absolute)
			{
				Matrix mat_parent_world = parent->GetTransform()->mat_world;
				Matrix mat_parent_scale = parent->GetTransform()->mat_world_scale;
				Matrix mat_parent_scale_inv = XMMatrixInverse(nullptr, mat_parent_scale);

				// 월드 = 로컬월드 * 부모크기 역 * 부모 월드(크기/회전/이동)
				mat_world = mat_world * mat_parent_scale_inv * mat_parent_world;
			}
			else
			{
				mat_world_scale = parent->GetTransform()->mat_world_scale;
				mat_world *= parent->GetTransform()->mat_world;
			}


			for (int i = 0; i < 3; ++i)
			{
				world_dir[i] = XMVector3TransformNormal(default_dir[i], mat_world);
				world_dir[i].Normalize();
			}
		}
	}

	void Transform::UpdateData()
	{
		// 위치값을 상수버퍼에 전달 및 바인딩		
		ConstBuffer* transform_buff = Device::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);

		Tansform_Matrix.mat_world = mat_world;
		Tansform_Matrix.mat_wv = Tansform_Matrix.mat_world * Tansform_Matrix.mat_view;
		Tansform_Matrix.mat_wvp = Tansform_Matrix.mat_wv * Tansform_Matrix.mat_proj;

		transform_buff->SetData(&Tansform_Matrix);
		transform_buff->UpdateData();
	}

	void Transform::SaveToLevelFile(FILE* _file)
	{
		fwrite(&relative_pos, sizeof(vec3), 1, _file);
		fwrite(&relative_scale, sizeof(vec3), 1, _file);
		fwrite(&relative_rotation, sizeof(vec3), 1, _file);
		fwrite(&absolute, sizeof(bool), 1, _file);
	}

	void Transform::LoadFromLevelFile(FILE* _file)
	{
		fread(&relative_pos, sizeof(vec3), 1, _file);
		fread(&relative_scale, sizeof(vec3), 1, _file);
		fread(&relative_rotation, sizeof(vec3), 1, _file);
		fread(&absolute, sizeof(bool), 1, _file);
	}
}