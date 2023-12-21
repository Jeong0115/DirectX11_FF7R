#pragma once

#include "UI.h"
namespace ff7r
{
	class SceneUI :
		public UI
	{
	public:
		virtual void FinalTick() override;
		virtual int RenderUpdate() override;

	public:
		SceneUI();
		virtual ~SceneUI();
	};

}