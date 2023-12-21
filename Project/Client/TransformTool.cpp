#include "pch.h"

#include <Engine/KeyMgr.h>
#include <Engine/Transform.h>
#include <Engine/TimeMgr.h>

#include "EditorObjMgr.h"
#include "GameObjectEx.h"
#include "TransformTool.h"

namespace ff7r
{
	TransformTool::TransformTool()
		: target(nullptr)
	{
	}
	void TransformTool::Tick()
	{
		if (target != nullptr)
		{
			Overlap();
		}
		else
		{
			return;

		}
		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::SCRIPT; ++i)
		{
			if (nullptr != GetComponent((COMPONENT_TYPE)i))
				GetComponent((COMPONENT_TYPE)i)->Tick();
		}

		const vector<GameObject*>& vecChild = GetChild();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			vecChild[i]->Tick();
		}

	}
	void TransformTool::FinalTick()
	{
		if (target != nullptr)
		{
			GetTransform()->SetRelativePos(target->GetTransform()->GetRelativePos());
			GetTransform()->SetRelativeRot(target->GetTransform()->GetRelativeRot());

			GameObjectEx* camera = EditorObjMgr::GetInst()->GetEditorCamera();
			vec3 cam_pos = camera->GetTransform()->GetRelativePos();	
			vec3 my_pos = GetTransform()->GetRelativePos();
			
			float factor = sqrt((cam_pos.x - my_pos.x) * (cam_pos.x - my_pos.x)
				+ (cam_pos.y - my_pos.y) * (cam_pos.y - my_pos.y) 
				+ (cam_pos.z - my_pos.z) * (cam_pos.z - my_pos.z));
			
			GetTransform()->SetRelativeScale(vec3(1.f, 1.f, 1.f) * factor / 200.f);
		}
		else
		{
			return;
		}

		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::SCRIPT; ++i)
		{
			if (nullptr != GetComponent((COMPONENT_TYPE)i))
			{
				GetComponent((COMPONENT_TYPE)i)->FinalTick();
			}
		}

		const vector<GameObject*>& vecChild = GetChild();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			vecChild[i]->FinalTick();
		}

	}
	void TransformTool::Render()
	{
		if (target != nullptr)
		{
			GameObject::Render();
		}
	}
	void TransformTool::Overlap()
	{
		float speed = 100.f;
		vec3 pos = target->GetTransform()->GetRelativePos();

		if (KEY_PRESSED(KEY::D))
		{
			pos.x += DT * speed;
		}

		if (KEY_PRESSED(KEY::A))
		{
			pos.x -= DT * speed;
		}

		if (KEY_PRESSED(KEY::W))
		{
			pos.z += DT * speed;
		}

		if (KEY_PRESSED(KEY::S))
		{
			pos.z -= DT * speed;
		}

		if (KEY_PRESSED(KEY::Q))
		{
			pos.y += DT * speed;
		}

		if (KEY_PRESSED(KEY::E))
		{
			pos.y -= DT * speed;
		}

		target->GetTransform()->SetRelativePos(pos);
	}
}