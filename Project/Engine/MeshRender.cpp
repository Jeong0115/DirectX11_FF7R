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

		// Transform �� UpdateData ��û
		GetTransform()->UpdateData();

		// Animator2D ������Ʈ�� �ִٸ�
		if (GetAnimator2D())
		{
			GetAnimator2D()->UpdateData();
		}

		// ���� ������Ʈ
		GetMaterial()->UpdateData();

		// ����
		GetMesh()->Render();

		// Animation ���� ���� ����
		if (GetAnimator2D())
			GetAnimator2D()->Clear();
	}
}