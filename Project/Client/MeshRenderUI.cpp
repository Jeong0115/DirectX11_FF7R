#include "pch.h"
#include "MeshRenderUI.h"

#include <Engine\MeshRender.h>
#include <Engine\ResourceMgr.h>

#include "ImGuiMgr.h"
#include "SearchListUI.h"
#include "TreeUI.h"


namespace ff7r
{
	MeshRenderUI::MeshRenderUI()
		: ComponentUI("##MeshRender", COMPONENT_TYPE::MESHRENDER)
	{
		SetName("MeshRender");
	}

	MeshRenderUI::~MeshRenderUI()
	{
	}


	int MeshRenderUI::RenderUpdate()
	{
		if (FALSE == ComponentUI::RenderUpdate())
			return FALSE;

		char name_buff[50] = {};

		Ptr<Mesh> mesh = GetTarget()->GetMeshRender()->GetMesh();
		Ptr<Material> material = GetTarget()->GetMeshRender()->GetMaterial();

		path res_name;
		if (mesh != nullptr)
		{
			res_name = mesh->GetKey();
			CopyStrToChar(res_name.stem().string(), name_buff, 50);
		}

		ImGui::Text("Mesh    ");
		ImGui::SameLine();
		ImGui::InputText("##MeshName", name_buff, 50, ImGuiInputTextFlags_ReadOnly);

		// Mesh 드랍 체크
		if (ImGui::BeginDragDropTarget())
		{
			// 해당 노드에서 마우스 뗀 경우, 지정한 PayLoad 키값이 일치한 경우
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Resource");
			if (payload)
			{
				TreeNode* node = (TreeNode*)payload->Data;
				Resource* resource = (Resource*)node->GetData();
				if (RES_TYPE::MESH == resource->GetType())
				{
					GetTarget()->GetMeshRender()->SetMesh((Mesh*)resource);
				}
			}

			ImGui::EndDragDropTarget();
		}


		ImGui::SameLine();
		if (ImGui::Button("##MeshSelectBtn", ImVec2(18, 18)))
		{
			SearchListUI* list_ui = (SearchListUI*)ImGuiMgr::GetInst()->FindUI("##SearchListUI");
			list_ui->Reset(SearchListUI::SearchListType::MESHES, ImVec2(300.f, 500.f));
			list_ui->AddDynamic_Select(this, (UI_DELEGATE_1)&MeshRenderUI::SelectMesh);
		}

		memset(name_buff, 0, sizeof(char) * 50);
		if (material != nullptr)
		{
			res_name = material->GetKey();
			CopyStrToChar(res_name.stem().string(), name_buff, 50);
		}

		ImGui::Text("Material");
		ImGui::SameLine();
		ImGui::InputText("##MtrlName", name_buff, 50, ImGuiInputTextFlags_ReadOnly);

		if (ImGui::BeginDragDropTarget())
		{
			// 해당 노드에서 마우스 뗀 경우, 지정한 PayLoad 키값이 일치한 경우
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Resource");
			if (payload)
			{
				TreeNode* node = (TreeNode*)payload->Data;
				Resource* resource = (Resource*)node->GetData();
				if (RES_TYPE::MATERIAL == resource->GetType())
				{
					GetTarget()->GetMeshRender()->SetMaterial((Material*)resource);
				}
			}

			ImGui::EndDragDropTarget();
		}


		ImGui::SameLine();

		if (ImGui::Button("##MtrlSelectBtn", ImVec2(18, 18)))
		{
			const map<wstring, Ptr<Resource>>& mapMtrl = ResourceMgr::GetInst()->GetResources(RES_TYPE::MATERIAL);

			SearchListUI* list_ui = (SearchListUI*)ImGuiMgr::GetInst()->FindUI("##SearchListUI");
			list_ui->Reset(SearchListUI::SearchListType::MATERIALS, ImVec2(300.f, 500.f));
			list_ui->AddDynamic_Select(this, (UI_DELEGATE_1)&MeshRenderUI::SelectMaterial);
		}

		return TRUE;
	}

	void MeshRenderUI::SelectMesh(DWORD_PTR _key)
	{
		string key = (char*)_key;
		Ptr<Mesh> mesh = ResourceMgr::GetInst()->FindRes<Mesh>(wstring(key.begin(), key.end()));
		GetTarget()->GetMeshRender()->SetMesh(mesh);
	}

	void MeshRenderUI::SelectMaterial(DWORD_PTR _key)
	{
		string key = (char*)_key;
		Ptr<Material> material = ResourceMgr::GetInst()->FindRes<Material>(wstring(key.begin(), key.end()));
		GetTarget()->GetMeshRender()->SetMaterial(material);
	}
}