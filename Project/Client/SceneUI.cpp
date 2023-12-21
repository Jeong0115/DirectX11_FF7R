#include "pch.h"
#include "SceneUI.h"

#include "PrefabUI.h"
#include "TreeUI.h"
#include <Engine/KeyMgr.h>
#include <Engine/GameEngine.h>
#include <Engine/Prefab.h>
#include <Engine/EventMgr.h>
namespace ff7r
{
	SceneUI::SceneUI()
		: UI("##Scene")
	{
		SetName("Scene");
	}

	SceneUI::~SceneUI()
	{
	}

	void SceneUI::FinalTick()
	{
		if (!IsActive())
			return;

		RenderUpdate();
	}

	int SceneUI::RenderUpdate()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		//style.Alpha = 0.0f;
		//ImGuiStyle& style = ImGui::GetStyle();	
		////style.Colors[ImGuiCol_WindowBg].w = 0.0f;
		//
		//for (int i = 0; i < (int)ImGuiCol_COUNT; i++)
		//{
		//	style.Colors[i].w = 0.0f;
		//}
		vec2 winResolution = Engine::GetInst()->GetWindowResolution();
		bool a = true;
		ImGui::SetNextWindowBgAlpha(0.0f);
		//ImGui::SetNextWindowSize(ImVec2(100,100));
		ImGui::Begin("My Fixed Window", 0/*, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings
			| ImGuiWindowFlags_NoInputs*/);
			//ImGui::BeginChild("abc", ImVec2(winResolution.x, winResolution.y));




			//ImGui::InvisibleButton("LargeDropArea", ImVec2(winResolution.x, winResolution.y));
		if (ImGui::BeginDragDropTarget())
		{
			// 해당 노드에서 마우스 뗀 경우, 지정한 PayLoad 키값이 일치한 경우
			const ImGuiPayload* pPayLoad = ImGui::AcceptDragDropPayload("Resource");
			if (pPayLoad)
			{
				TreeNode* pNode = (TreeNode*)pPayLoad->Data;
				Resource* pRes = (Resource*)pNode->GetData();
				if (RES_TYPE::PREFAB == pRes->GetType())
				{
					//GetTarget()->MeshRender()->SetMesh((CMesh*)pRes);
					vec2 winResolution = Engine::GetInst()->GetWindowResolution();
					vec2 mousePos = KeyMgr::GetInst()->GetMousePos();

					if (mousePos.x > 0 && mousePos.y > 0
						&& mousePos.x < winResolution.x && mousePos.y < winResolution.y)
					{
						Prefab* prefab = dynamic_cast<Prefab*>(pRes);

						Event instantiate;
						instantiate.type = EVENT_TYPE::CREATE_OBJECT;
						instantiate.w_param = (DWORD_PTR)prefab->Instantiate();
						instantiate.l_param = (DWORD_PTR)3;

						EventMgr::GetInst()->AddEvent(instantiate);
					}
				}
			}

			ImGui::EndDragDropTarget();
		}
		//ImGui::EndChild();
		ImGui::End();
		style.Alpha = 1.0f;
		return 0;

	}

}