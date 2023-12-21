#pragma once

#include "UI.h"

#include <Engine\ptr.h>

class Texture;
namespace ff7r
{
	class DeferredUI : public UI
	{
	public:
		DeferredUI();
		~DeferredUI();

		virtual void Init() override;
		virtual int RenderUpdate() override;

	private:
		Ptr<Texture> diffuse_tex;
		Ptr<Texture> normal_tex;
		Ptr<Texture> position_tex;
		Ptr<Texture> data_tex;
	};
}

