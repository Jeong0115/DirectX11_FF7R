#include "pch.h"

#include "Animator2D.h"
#include "Transform.h"

#include "MeshRender.h"
namespace ff7r
{
	MeshRender::MeshRender()
		: RenderComponent(COMPONENT_TYPE::MESHRENDER)
	{
	}

	MeshRender::~MeshRender()
	{
	}

	void MeshRender::FinalTick()
	{
	}

	void MeshRender::Render()
	{
		if (nullptr == GetMesh() || nullptr == GetMaterial())
			return;

		// Transform 에 UpdateData 요청
		GetTransform()->UpdateData();

		// Animator2D 컴포넌트가 있다면
		if (GetAnimator2D())
		{
			GetAnimator2D()->UpdateData();
		}

		// 재질 업데이트
		GetMaterial()->UpdateData();

		// 렌더
		GetMesh()->Render();

		// Animation 관련 정보 제거
		if (GetAnimator2D())
			GetAnimator2D()->Clear();
	}
}