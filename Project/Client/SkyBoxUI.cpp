#include "pch.h"

#include <Engine\SkyBox.h>
#include <Engine\ResourceMgr.h>

#include "ImGuiMgr.h"
#include "SearchListUI.h"
#include "TreeUI.h"

#include "SkyBoxUI.h"

namespace ff7r
{
	SkyBoxUI::SkyBoxUI()
		: ComponentUI("##SkyBox", COMPONENT_TYPE::SKYBOX)
	{
		SetName("SkyBox");
	}
	SkyBoxUI::~SkyBoxUI()
	{
	}
	int SkyBoxUI::RenderUpdate()
	{
		if (FALSE == ComponentUI::RenderUpdate())
			return FALSE;

		char szBuff[50] = {};

		Ptr<Mesh> mesh = GetTarget()->GetSkyBox()->GetMesh();

		ImGui::Text("Mesh    ");
		ImGui::SameLine();
		GetResKey(mesh.Get(), szBuff, 50);
		ImGui::InputText("##MeshName", szBuff, 50, ImGuiInputTextFlags_ReadOnly);

		// Mesh 드랍 체크
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Resource");
			if (payload)
			{
				TreeNode* node = (TreeNode*)payload->Data;
				Resource* res = (Resource*)node->GetData();
				if (RES_TYPE::MESH == res->GetType())
				{
					GetTarget()->GetSkyBox()->SetMesh((Mesh*)res);
				}
			}

			ImGui::EndDragDropTarget();
		}


		ImGui::SameLine();
		if (ImGui::Button("##MeshSelectBtn", ImVec2(18, 18)))
		{
			SearchListUI* pListUI = (SearchListUI*)ImGuiMgr::GetInst()->FindUI("##SearchListUI");
			pListUI->Reset(SearchListUI::SearchListType::MESHES, ImVec2(300.f, 500.f));
			pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&SkyBoxUI::SelectMesh);
		}
	}

	void SkyBoxUI::SelectMesh(DWORD_PTR _key)
	{
		string key = (char*)_key;
		Ptr<Mesh> mesh = ResourceMgr::GetInst()->FindRes<Mesh>(wstring(key.begin(), key.end()));
		GetTarget()->GetSkyBox()->SetMesh(mesh);
	}
}