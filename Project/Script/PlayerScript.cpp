#include "pch.h"

#include <Engine\MeshRender.h>
#include <Engine\Material.h>

#include "PlayerScript.h"
namespace ff7r
{
	PlayerScript::PlayerScript()
		: Script((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
		, speed(500.f)
	{
		AddScriptParam(SCRIPT_PARAM::FLOAT, &speed, "Player Speed");
	}

	PlayerScript::~PlayerScript()
	{

	}

	void PlayerScript::Begin()
	{
		// 동적 재질 생성
		GetMeshRender()->GetDynamicMaterial();
	}

	void PlayerScript::Tick()
	{
		vec3 cur_position = GetTransform()->GetRelativePos();

		if (KEY_PRESSED(KEY::UP))
		{
			for (int i = 0; i < 4; ++i)
			{
				cur_position.y += DT * speed;
			}
		}

		if (KEY_PRESSED(KEY::DOWN))
		{
			for (int i = 0; i < 4; ++i)
			{
				cur_position.y -= DT * speed;
			}
		}

		if (KEY_PRESSED(KEY::LEFT))
		{
			for (int i = 0; i < 4; ++i)
			{
				cur_position.x -= DT * speed;
			}
		}

		if (KEY_PRESSED(KEY::RIGHT))
		{
			for (int i = 0; i < 4; ++i)
			{
				cur_position.x += DT * speed;
			}
		}

		if (KEY_PRESSED(KEY::Z))
		{
			vec3 rotation = GetTransform()->GetRelativeRot();
			rotation.z += DT * XM_PI;
			GetTransform()->SetRelativeRot(rotation);
		}

		GetTransform()->SetRelativePos(cur_position);

		if (KEY_TAP(KEY::SPACE))
		{
			DrawDebugCircle(GetTransform()->GetWorldPos(), 500.f, vec4(0.f, 0.f, 1.f, 1.f), vec3(0.f, 0.f, 0.f), 2.f);

			Shoot();
		}
	}

	void PlayerScript::Shoot()
	{
		// 미사일 프리팹 참조
		//Ptr<Prefab> missile = ResourceMgr::GetInst()->FindRes<Prefab>(L"MissilePrefab");
		//vec3 vMissilePos = Transform()->GetRelativePos() + vec3(0.f, 0.5f, 0.f) * Transform()->GetRelativeScale();
		//GameObject* clone = missile->Instantiate();
		//
		//// 레벨에 추가
		//SpawnGameObject(clone, vMissilePos, L"PlayerProjectile");
	}

	void PlayerScript::BeginOverlap(Collider2D* _other)
	{
		GameObject* other_object = _other->GetOwner();

		if (other_object->GetName() == L"Monster")
		{
			DestroyObject(other_object);
		}
	}

	void PlayerScript::SaveToLevelFile(FILE* _File)
	{
		fwrite(&speed, sizeof(float), 1, _File);
	}

	void PlayerScript::LoadFromLevelFile(FILE* _File)
	{
		fread(&speed, sizeof(float), 1, _File);
	}
}