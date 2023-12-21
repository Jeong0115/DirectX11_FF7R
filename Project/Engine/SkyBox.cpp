#include "pch.h"

#include "ResourceMgr.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"

#include "SkyBox.h"
namespace ff7r
{
	SkyBox::SkyBox()
		: RenderComponent(COMPONENT_TYPE::SKYBOX)
		, type(SKYBOX_TYPE::NONE)
	{
		SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"SkyBoxMtrl"));
	}
	SkyBox::~SkyBox()
	{
	}

	void SkyBox::FinalTick()
	{
		if (texture != nullptr)
		{
			if (texture->IsCubeTex() && (type != SKYBOX_TYPE::CUBE))
			{
				type = SKYBOX_TYPE::CUBE;
				SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"CubeMesh"));
			}
			else if (!texture->IsCubeTex() && (type != SKYBOX_TYPE::SPHERE))
			{
				type = SKYBOX_TYPE::SPHERE;
				SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"SphereMesh"));
			}
		}
		else
		{
			type = SKYBOX_TYPE::NONE;
		}
	}

	void SkyBox::Render()
	{
		if (nullptr == GetMesh() || nullptr == GetMaterial())
			return;

		GetTransform()->UpdateData();

		//switch (type)
		//{
		//case SKYBOX_TYPE::CUBE:
		//	GetMaterial()->SetTexParam(TEX_CUBE_0, texture);
		//	GetMaterial()->SetScalarParam(INT_0, &type);
		//	break;
		//
		//case SKYBOX_TYPE::SPHERE:
		//	GetMaterial()->SetTexParam(TEX_0, texture);
		//	GetMaterial()->SetScalarParam(INT_0, &type);
		//	break;
		//
		//default: break;
		//}

		GetMaterial()->UpdateData();
		GetMesh()->Render();
	}

	void SkyBox::SetSkyBoxTexture(Ptr<Texture> _tex)
	{
		texture = _tex;

		if (texture->IsCubeTex())
		{
			type = SKYBOX_TYPE::CUBE;
			SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"CubeMesh"));
		}
		else
		{
			type = SKYBOX_TYPE::SPHERE;
			SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"SphereMesh"));
		}
	}
}