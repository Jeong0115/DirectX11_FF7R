#include "pch.h"

#include "KeyMgr.h"
#include "RenderMgr.h"
#include "TimeMgr.h"
#include "Transform.h"

#include "Light2D.h"
namespace ff7r
{
	Light2D::Light2D()
		: Component(COMPONENT_TYPE::LIGHT2D)
	{
	}

	Light2D::~Light2D()
	{
	}

	void Light2D::FinalTick()
	{
		if (KEY_PRESSED(KEY::C))
		{
			vec3 rotaion = GetTransform()->GetRelativeRot();
			rotaion.z += DT * (XM_PI / 2.f);
			GetTransform()->SetRelativeRot(rotaion);
		}

		light_info.world_pos = GetTransform()->GetWorldPos();
		light_info.world_dir = GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);

		RenderMgr::GetInst()->RegisterLight2D(light_info);
	}

	void Light2D::SaveToLevelFile(FILE* _file)
	{
		fwrite(&light_info, sizeof(LightInfo), 1, _file);
	}

	void Light2D::LoadFromLevelFile(FILE* _file)
	{
		fread(&light_info, sizeof(LightInfo), 1, _file);
	}
}