#pragma once
#include <Engine\Script.h>

namespace ff7r
{
	class MonsterScript : public Script
	{
	public:
		MonsterScript();
		~MonsterScript();

		CLONE(MonsterScript);

		virtual void Tick() override;
		virtual void BeginOverlap(Collider2D* _Other) override;

	};
}

