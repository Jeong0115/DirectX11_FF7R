#pragma once

#include <Engine/GameObject.h>

namespace ff7r
{
	class TransformTool : public GameObject
	{
	public:
		TransformTool();

		virtual void Tick() override;
		virtual void FinalTick() override;
		virtual void Render() override;

		void Overlap();

		void SetTarget(GameObject* _target) { target = _target; }
		void Clear() { target = nullptr; }

	private:
		GameObject* target;
	};
}
