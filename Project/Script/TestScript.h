#pragma once
#include "Engine\Script.h"

namespace ff7r
{
	class TestScript : public Script
	{
	public:
		TestScript();
		~TestScript();

		CLONE(TestScript);

		virtual void Begin() override;
		virtual void Tick() override;

		virtual void SaveToLevelFile(FILE* _file) override;
		virtual void LoadFromLevelFile(FILE* _file) override;
	};
}

