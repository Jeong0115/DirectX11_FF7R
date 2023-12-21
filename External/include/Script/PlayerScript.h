#pragma once

#include <Engine\Script.h>
namespace ff7r
{
	class PlayerScript : public Script
	{
	public:
		PlayerScript();
		~PlayerScript();

		CLONE(PlayerScript);

		virtual void Begin() override;
		virtual void Tick() override;

		virtual void BeginOverlap(Collider2D* _other) override;
		virtual void SaveToLevelFile(FILE* _file) override;
		virtual void LoadFromLevelFile(FILE* _fILE) override;

	private:
		void Shoot();

		float speed;
	};
}
