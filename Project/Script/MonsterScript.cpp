#include "pch.h"
#include "MonsterScript.h"

namespace ff7r
{
	MonsterScript::MonsterScript()
		: Script((UINT)SCRIPT_TYPE::MONSTERSCRIPT)
	{
	}

	MonsterScript::~MonsterScript()
	{
	}

	void MonsterScript::Tick()
	{
	}

	void MonsterScript::BeginOverlap(Collider2D* _other)
	{
		if (L"Parent Object" == _other->GetOwner()->GetName())
		{
			//Destroy();	

			SetLifeSpan(2.f);
		}
	}
}