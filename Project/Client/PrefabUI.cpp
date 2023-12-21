#include "pch.h"
#include "PrefabUI.h"
#include "TreeUI.h"
#include <Engine/KeyMgr.h>
#include <Engine/GameEngine.h>
#include <Engine/Prefab.h>
#include <Engine/EventMgr.h>
namespace ff7r
{
	PrefabUI::PrefabUI()
		:ResourceUI(RES_TYPE::PREFAB)
	{
		SetName("Prefab");
	}

	PrefabUI::~PrefabUI()
	{

	}

	int PrefabUI::RenderUpdate()
	{
		ResourceUI::RenderUpdate();

		if (ImGui::BeginDragDropSource())
		{
			//ImGui::SetDragDropPayload(m_Owner->m_strDragDropID.c_str(), this, sizeof(TreeNode));
			//ImGui::Text(m_strName.c_str());
			//
			//// Tree 에 드래그 노드 등록
			//m_Owner->SetDragNode(this);

			ImGui::EndDragDropSource();
		}
		//char szBuff[50] = {};

		//Ptr<CMesh> pMesh = GetTarget()->MeshRender()->GetMesh();
		//Ptr<CMaterial> pMtrl = GetTarget()->MeshRender()->GetMaterial();

		//ImGui::Text("Mesh    ");
		//ImGui::SameLine();
		//GetResKey(pMesh.Get(), szBuff, 50);
		//ImGui::InputText("##MeshName", szBuff, 50, ImGuiInputTextFlags_ReadOnly);

		//// Mesh 드랍 체크
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


		ImGui::SameLine();
		return 0;
	}
}