#include "pch.h"

#include "RenderComponent.h"
namespace ff7r
{
	RenderComponent::RenderComponent(COMPONENT_TYPE _type)
		: Component(_type)
		, mesh(nullptr)
		, shaderd_mtrl(nullptr)
		, dynamic_mtrl(nullptr)
		, current_mtrl(nullptr)
	{
	}

	RenderComponent::~RenderComponent()
	{
	}

	void RenderComponent::SetMaterial(Ptr<Material> _mtrl)
	{
		shaderd_mtrl = _mtrl;
		current_mtrl = shaderd_mtrl;
	}

	Ptr<Material> RenderComponent::GetDynamicMaterial()
	{
		// 원본 재질이 없다 -> Nullptr 반환
		if (nullptr == shaderd_mtrl)
		{
			current_mtrl = nullptr;
			return current_mtrl;
		}

		// 동적 재질 최초 요청시 제작 해서 준다.
		if (nullptr == dynamic_mtrl)
		{
			dynamic_mtrl = shaderd_mtrl->Clone();
		}

		// 동적 재질을 현재 사용재질로 지정한다.
		current_mtrl = dynamic_mtrl;

		return current_mtrl;
	}

	void RenderComponent::SaveToLevelFile(FILE* _file)
	{
		SaveResRef(mesh.Get(), _file);
		SaveResRef(shaderd_mtrl.Get(), _file);
	}

	void RenderComponent::LoadFromLevelFile(FILE* _file)
	{
		LoadResRef(mesh, _file);
		LoadResRef(shaderd_mtrl, _file);

		SetMaterial(shaderd_mtrl);
	}
}
