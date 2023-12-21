#include "pch.h"
#include "ContentUI.h"

#include <Engine/ResourceMgr.h>
#include <Engine/PathMgr.h>
#include <Engine/EventMgr.h>

#include <Engine/LevelRes.h>
#include <Engine/GameObject.h>

#include <Engine/Prefab.h>

#include "OptionListUI.h"
#include "LevelSaveLoad.h"
#include "TreeUI.h"
#include "ImGuiMgr.h"
#include "InspectorUI.h"


namespace ff7r
{
	ContentUI::ContentUI()
		: UI("##Content")
	{
		SetName("Content");

		// ContentUI 안에 자식으로 Tree 를 추가한다.
		tree = new TreeUI;
		tree->SetName("ContentTree");
		tree->SetActive(true);
		tree->ShowRoot(false);

		tree->AddDynamic_Option(this, (UI_DELEGATE_1)&ContentUI::OpenOptionList);
		tree->AddDynamic_Select(this, (UI_DELEGATE_1)&ContentUI::SetTargetToInspector);
		tree->AddDynamic_DoubleClik(this, (UI_DELEGATE_1)&ContentUI::DoubleClikContent);
		tree->SetDragDropID("Resource");
		AddChildUI(tree);
	}

	ContentUI::~ContentUI()
	{

	}

	void ContentUI::Init()
	{
		Reload();
	}

	void ContentUI::Tick()
	{
		UI::Tick();

		if (ResourceMgr::GetInst()->IsResourceChanged())
		{
			ResetContent();
		}
	}

	void ContentUI::FinalTick()
	{
		//UI::finaltick();

		if (!is_active)
			return;

		string strFullName = name + id;
		ImGui::Begin(strFullName.c_str(), &is_active);

		RenderUpdate();

		for (size_t i = 0; i < child_ui.size(); ++i)
		{
			// 자식UI 가 비활성화 상태면 건너뛴다.
			if (!child_ui[i]->IsActive())
				continue;

			child_ui[i]->FinalTick();

			// 자식 UI 간의 구분선
			if (i != child_ui.size() - 1)
				ImGui::Separator();
		}

		if (ImGui::BeginDragDropTarget())
		{
			// 해당 노드에서 마우스 뗀 경우, 지정한 PayLoad 키값이 일치한 경우
			const ImGuiPayload* pPayLoad = ImGui::AcceptDragDropPayload("GameObject");
			if (pPayLoad)
			{
				TreeNode* pNode = (TreeNode*)pPayLoad->Data;
				GameObject* pGameObject = (GameObject*)pNode->GetData();

				Ptr<Prefab> prefab = new Prefab;
				ResourceMgr::GetInst()->AddRes<Prefab>(L"prefab\\" + pGameObject->GetName() + L".pref", prefab);
				prefab->RegisterProtoObject(pGameObject->Clone());
				prefab->Save(prefab->GetKey());

				Event instantiate;
				instantiate.type = EVENT_TYPE::DELETE_OBJECT;
				instantiate.w_param = (DWORD_PTR)pGameObject;

				InspectorUI* inspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
				if (inspector->GetTargetObject() == pGameObject)
				{
					inspector->SetTargetObject(nullptr);
				}


				EventMgr::GetInst()->AddEvent(instantiate);
			}

			ImGui::EndDragDropTarget();
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1) && !tree->IsChildClicked())
		{
			OptionListUI* option = (OptionListUI*)ImGuiMgr::GetInst()->FindUI("##Option");
			option->OpenOption(OptionType::CONTENT);
		}

		tree->SetChildClicked(false);
		ImGui::End();
	}

	int ContentUI::RenderUpdate()
	{
		return 0;
	}

	void ContentUI::Reload()
	{
		// Content 폴더에 있는 파일 이름들을 확인
		res_path.clear();
		wstring strContentPath = PathMgr::GetInst()->GetContentPath();
		FindFileName(strContentPath);

		// 파일명으로 리소스 로딩
		for (size_t i = 0; i < res_path.size(); ++i)
		{
			RES_TYPE type = GetResTypeByExt(res_path[i]);

			if (type == RES_TYPE::END)
				continue;

			switch (type)
			{
			case RES_TYPE::LEVEL:
				ResourceMgr::GetInst()->Load<LevelRes>(res_path[i], res_path[i]);
				break;
			case RES_TYPE::MESHDATA:

				break;
			case RES_TYPE::MATERIAL:
				ResourceMgr::GetInst()->Load<Material>(res_path[i], res_path[i]);
				break;
			case RES_TYPE::PREFAB:
				ResourceMgr::GetInst()->Load<Prefab>(res_path[i], res_path[i]);
				break;
			case RES_TYPE::MESH:

				break;
			case RES_TYPE::TEXTURE:
				ResourceMgr::GetInst()->Load<Texture>(res_path[i], res_path[i]);
				break;
			case RES_TYPE::SOUND:
				ResourceMgr::GetInst()->Load<Sound>(res_path[i], res_path[i]);
				break;
			}
		}


		// 리소스의 원본파일 체크
		for (UINT i = 0; i < UINT(RES_TYPE::END); ++i)
		{
			const map<wstring, Ptr<Resource>>& mapRes = ResourceMgr::GetInst()->GetResources((RES_TYPE)i);

			for (const auto& pair : mapRes)
			{
				if (pair.second->IsEngineRes())
					continue;

				wstring strFilePath = strContentPath + pair.first;
				if (!filesystem::exists(strFilePath))
				{
					Event evn = {};
					evn.type = EVENT_TYPE::DELETE_RESOURCE;
					evn.w_param = (DWORD_PTR)i;
					evn.l_param = (DWORD_PTR)pair.second.Get();
					EventMgr::GetInst()->AddEvent(evn);
				}
			}
		}

		// 트리 갱신
		ResetContent();
	}


	void ContentUI::ResetContent()
	{
		// Tree Clear
		tree->Clear();
		tree->AddItem("Root", 0);

		// 리소스 매니저에서 현재 모든 리소스 목록 받아옴
		for (size_t i = 0; i < (UINT)RES_TYPE::END; ++i)
		{
			const map<wstring, Ptr<Resource>>& mapRes = ResourceMgr::GetInst()->GetResources((RES_TYPE)i);

			// m_Tree 에 현재 리소스 목록을 AddItem
			TreeNode* category = tree->AddItem(ToString((RES_TYPE)i), 0);
			category->SetCategoryNode(true);

			for (const auto& pair : mapRes)
			{
				path node_name = pair.first;
				tree->AddItem(node_name.stem().string(), (DWORD_PTR)pair.second.Get(), category);
			}
		}

	}

	void ContentUI::SetTargetToInspector(DWORD_PTR _SelectedNode)
	{
		TreeNode* pSelectedNode = (TreeNode*)_SelectedNode;
		Resource* pSelectObject = (Resource*)pSelectedNode->GetData();

		if (nullptr == pSelectObject)
			return;

		// Inspector 에 선택된 Resource 를 알려준다.	
		if (dynamic_cast<Prefab*>(pSelectObject))
		{
			InspectorUI* pInspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
			pInspector->SetTargetObject(((Prefab*)pSelectObject)->GetOriginalData());

			//InspectorUI* pInspector3 = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
			//pInspector3->SetTargetResource(pSelectObject);
		}
		else
		{
			InspectorUI* pInspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
			pInspector->SetTargetResource(pSelectObject);
		}
	}

	void ContentUI::DoubleClikContent(DWORD_PTR _SelectedNode)
	{
		TreeNode* pSelectedNode = (TreeNode*)_SelectedNode;
		Resource* pSelectObject = (Resource*)pSelectedNode->GetData();

		switch (pSelectObject->GetType())
		{
		case RES_TYPE::LEVEL:
		{
			/*CLevel* pLoadedLevel = */
			LevelRes* selectLevel = dynamic_cast<LevelRes*>(pSelectObject);

			Event evn = {};
			evn.type = EVENT_TYPE::LEVEL_CHANGE;
			evn.w_param = (DWORD_PTR)LevelSaveLoad::LoadLevel(selectLevel->GetKey());

			EventMgr::GetInst()->AddEvent(evn);
		}
		break;

		default: break;
		}
	}

	void ContentUI::OpenOptionList(DWORD_PTR _SelectedNode)
	{
		TreeNode* pSelectedNode = (TreeNode*)_SelectedNode;
		Resource* pSelectObject = (Resource*)pSelectedNode->GetData();

		OptionListUI* pInspector = (OptionListUI*)ImGuiMgr::GetInst()->FindUI("##Option");
		pInspector->OpenOption(OptionType::RESOURCE ,pSelectedNode);
	}


	void ContentUI::FindFileName(const wstring& _FolderPath)
	{
		WIN32_FIND_DATA FindData = {};

		wstring FolderPath = _FolderPath + L"*.*";

		HANDLE hFindHandle = FindFirstFile(FolderPath.c_str(), &FindData);

		while (FindNextFile(hFindHandle, &FindData))
		{
			if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (!wcscmp(FindData.cFileName, L".."))
				{
					continue;
				}

				FindFileName(_FolderPath + FindData.cFileName + L"\\");
				continue;
			}

			wstring strContentPath = PathMgr::GetInst()->GetContentPath();
			wstring RelativePath = _FolderPath + FindData.cFileName;
			RelativePath = RelativePath.substr(strContentPath.length(), RelativePath.length() - strContentPath.length());

			res_path.push_back(RelativePath);
		}

		FindClose(hFindHandle);
	}

	RES_TYPE ContentUI::GetResTypeByExt(const wstring& _relativepath)
	{
		wchar_t szExt[50] = {};
		_wsplitpath_s(_relativepath.c_str(), 0, 0, 0, 0, 0, 0, szExt, 50);
		wstring strExt = szExt;

		if (L".lv" == strExt)
			return RES_TYPE::LEVEL;
		else if (L".mdat" == strExt)
			return RES_TYPE::MESHDATA;
		else if (L".pref" == strExt)
			return RES_TYPE::PREFAB;
		else if (L".mesh" == strExt)
			return RES_TYPE::MESH;
		else if (L".mtrl" == strExt)
			return RES_TYPE::MATERIAL;
		else if (L".png" == strExt || L".jpg" == strExt
			|| L".jpeg" == strExt || L".bmp" == strExt
			|| L".tga" == strExt || L".dds" == strExt)
			return RES_TYPE::TEXTURE;
		else if (L".mp3" == strExt || L".wav" == strExt || L".oga" == strExt)
			return RES_TYPE::SOUND;
		else
			return RES_TYPE::END;
	}

}