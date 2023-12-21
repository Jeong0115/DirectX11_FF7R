#include "pch.h"

#include <Engine/GameObject.h>
#include <Engine/Material.h>
#include <Engine/ResourceMgr.h>
#include <Engine/Transform.h>

#include "OutlinerUI.h"
#include "ImGuiMgr.h"
#include "ImGuiEventMgr.h"

namespace ff7r
{
	ImGuiEventMgr::ImGuiEventMgr()
	{
	}

	ImGuiEventMgr::~ImGuiEventMgr()
	{
	}

	void ImGuiEventMgr::Tick()
	{
		for (int i = 0; i < events.size(); i++)
		{
			switch (events[i].event_type)
			{
			case IMGUI_EVENT_TYPE::CREATE:
			{
				CreateEmpty(events[i]);
			}
			break;

			case IMGUI_EVENT_TYPE::DELETE_1:
				break;

			case IMGUI_EVENT_TYPE::RENAME:
			{
				Resource* target = (Resource*)events[i].target;

				path old_path = target->GetKey();
				path new_path = old_path.parent_path().string() + "\\" + events[i].desc + old_path.extension().string();

				target->SetName(new_path.filename().wstring());

				ResourceMgr::GetInst()->ChangeResKey(target->GetKey(), new_path.wstring(), target);
			}
			break;
			}
		}
		events.clear();
	}
	
	void ImGuiEventMgr::CreateEmpty(ImguiEvent& _event)
	{
		switch (_event.res_type)
		{
			case RES_TYPE::MATERIAL:
			{
				Ptr<Material> material = new Material;
				ResourceMgr::GetInst()->AddRes<Material>(L"material\\New Material.mtrl", material);
				material->SetName(L"New Material");
				material->Save(material->GetKey());
			}
			break;

			case RES_TYPE::END:
			{
				GameObject* object = new GameObject;
				object->AddComponent(new Transform);
				object->SetName(L"GameObject");

				if (_event.desc == "UI")
				{
					SpawnGameObject(object, vec3(0.f, 0.f, 0.f), L"UI");
				}
				else
				{
					SpawnGameObject(object, vec3(0.f, 0.f, 0.f), L"Default");
				}

				OutlinerUI* outliner = (OutlinerUI*)ImGuiMgr::GetInst()->FindUI("##Outliner");
				outliner->SetSelectedNodeData(DWORD_PTR(object));
			}
			break;
		}
	}
}