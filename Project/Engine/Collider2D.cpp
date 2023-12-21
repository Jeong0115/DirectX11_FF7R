#include "pch.h"
#include "components.h"

#include "Collider2D.h"
#include "Script.h"
namespace ff7r
{
	Collider2D::Collider2D()
		: Component(COMPONENT_TYPE::COLLIDER2D)
		, type(COLLIDER2D_TYPE::RECT)
		, is_absolute(false)
		, collision_cnt(0)
	{
		SetName(L"Collider2D");
	}

	Collider2D::~Collider2D()
	{
	}

	void Collider2D::FinalTick()
	{
		// 충돌 회수가 음수인 경우
		assert(0 <= collision_cnt);

		mat_world_colli = XMMatrixScaling(offset_scale.x, offset_scale.y, offset_scale.z);
		mat_world_colli *= XMMatrixTranslation(offset_pos.x, offset_pos.y, offset_pos.z);

		const Matrix& mat_world = GetTransform()->GetWorldMat();

		if (is_absolute)
		{
			Matrix matParentScaleInv = XMMatrixInverse(nullptr, GetTransform()->GetWorldScaleMat());
			mat_world_colli = mat_world_colli * matParentScaleInv * mat_world;
		}
		else
		{
			// 충돌체 월드 * 오브젝트 월드
			mat_world_colli *= mat_world;
		}

		// DebugShape 요청
		vec4 color = vec4(0.f, 1.f, 0.f, 1.f);
		if (0 < collision_cnt)
			color = vec4(1.f, 0.f, 0.f, 1.f);

		if (COLLIDER2D_TYPE::CIRCLE == type)
			DrawDebugCircle(mat_world_colli, color, 0.f);
		else
			DrawDebugRect(mat_world_colli, color, 0.f);
	}

	void Collider2D::BeginOverlap(Collider2D* _other)
	{
		collision_cnt += 1;

		// Script 호출
		const vector<Script*>& scripts = GetOwner()->GetScripts();
		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->BeginOverlap(_other);
		}
	}

	void Collider2D::OnOverlap(Collider2D* _other)
	{
		// Script 호출
		const vector<Script*>& scripts = GetOwner()->GetScripts();
		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->OnOverlap(_other);
		}
	}

	void Collider2D::EndOverlap(Collider2D* _other)
	{
		collision_cnt -= 1;

		// Script 호출
		const vector<Script*>& scripts = GetOwner()->GetScripts();
		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->EndOverlap(_other);
		}
	}

	void Collider2D::SaveToLevelFile(FILE* _file)
	{
		fwrite(&offset_pos, sizeof(vec3), 1, _file);
		fwrite(&offset_scale, sizeof(vec3), 1, _file);
		fwrite(&is_absolute, sizeof(bool), 1, _file);
		fwrite(&type, sizeof(UINT), 1, _file);
	}

	void Collider2D::LoadFromLevelFile(FILE* _file)
	{
		fread(&offset_pos, sizeof(vec3), 1, _file);
		fread(&offset_scale, sizeof(vec3), 1, _file);
		fread(&is_absolute, sizeof(bool), 1, _file);
		fread(&type, sizeof(UINT), 1, _file);
	}
}