#include "pch.h"

#include "RenderMgr.h"
#include "TimeMgr.h"
#include "Transform.h"
#include "KeyMgr.h"

#include "Light3D.h"

namespace ff7r
{
	Light3D::Light3D()
		: Component(COMPONENT_TYPE::LIGHT3D)
	{
	}
	Light3D::~Light3D()
	{
	}

	void Light3D::FinalTick()
	{
		if (KEY_PRESSED(KEY::C))
		{
			vec3 rotaion = GetTransform()->GetRelativeRot();
			rotaion.z += DT * (XM_PI / 2.f);
			GetTransform()->SetRelativeRot(rotaion);
		}

		light_info.world_pos = GetTransform()->GetWorldPos();
		light_info.world_dir = GetTransform()->GetWorldDir(DIR_TYPE::FRONT);

		RenderMgr::GetInst()->RegisterLight3D(light_info);
	}

	void Light3D::SaveToLevelFile(FILE* _file)
	{
		fwrite(&light_info, sizeof(LightInfo), 1, _file);
	}

	void Light3D::LoadFromLevelFile(FILE* _file)
	{
		fread(&light_info, sizeof(LightInfo), 1, _file);
	}
}