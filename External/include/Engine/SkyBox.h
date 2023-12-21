#pragma once

#include "RenderComponent.h"

namespace ff7r
{
	enum class SKYBOX_TYPE
	{
		SPHERE,
		CUBE,
		NONE
	};

	class SkyBox : public RenderComponent
	{
	public:
		SkyBox();
		~SkyBox();

		CLONE(SkyBox);

		virtual void FinalTick() override;
		virtual void Render() override;

		void SetSkyBoxTexture(Ptr<Texture> _tex);

	private:
		SKYBOX_TYPE     type;
		Ptr<Texture>	texture;
	};
}

