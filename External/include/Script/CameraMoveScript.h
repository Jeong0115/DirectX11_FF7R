#pragma once
#include <Engine\Script.h>

namespace ff7r
{
	class CameraMoveScript : public Script
	{
	public:
		CameraMoveScript();
		~CameraMoveScript();

		virtual void Tick() override;

	private:
		CLONE(CameraMoveScript);

		void Camera2DMove();
		void Camera3DMove();

		float camera_speed;
	};
}

