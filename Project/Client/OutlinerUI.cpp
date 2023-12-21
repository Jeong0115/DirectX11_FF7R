#include "pch.h"
#include "OutlinerUI.h"

#include "ImGuiMgr.h"
#include "InspectorUI.h"
#include "OptionListUI.h"
#include "EditorObjMgr.h"

#include <Engine\EventMgr.h>
#include <Engine\LevelMgr.h>
#include <Engine\Level.h>
#include <Engine\Layer.h>
#include <Engine\GameObject.h>

#include "TreeUI.h"
namespace ff7r
{
	OutlinerUI::OutlinerUI()
		: UI("##Outliner")
		, tree(nullptr)
	{
		SetName("Outliner");

		// OutlinerUI �ȿ� �ڽ����� Tree �� �߰��Ѵ�.
		tree = new TreeUI;
		tree->SetName("OutlinerTree");
		tree->SetActive(true);
		tree->ShowRoot(false);

		tree->AddDynamic_Option(this, (UI_DELEGATE_1)&OutlinerUI::OpenOptionList);
		tree->AddDynamic_Select(this, (UI_DELEGATE_1)&OutlinerUI::SetTargetToInspector);
		tree->AddDynamic_DragDrop(this, (UI_DELEGATE_2)&OutlinerUI::DragDrop);
		tree->SetDragDropID("GameObject");


		AddChildUI(tree);
	}

	OutlinerUI::~OutlinerUI()
	{
	}

	void OutlinerUI::Tick()
	{
		if (EventMgr::GetInst()->IsLevelChanged())
		{
			ResetOutliner();

			// Ʈ�� ���� ��, ���� ���ó��� ������ DATA �� ������, ã�Ƽ� ���ó��� ����
			if (0 != m_dwSelectedData)
			{
				tree->GetSelectedNode(m_dwSelectedData);
			}
		}
	}

	void OutlinerUI::FinalTick()
	{
		string full_name = name + id;

		ImGui::Begin(full_name.c_str(), &is_active);

		RenderUpdate();

		for (size_t i = 0; i < child_ui.size(); ++i)
		{
			if (!child_ui[i]->IsActive())
			{
				continue;
			}

			child_ui[i]->FinalTick();

			if (i != child_ui.size() - 1)
			{
				ImGui::Separator();
			}
		}


		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1) && !tree->IsChildClicked())
		{
			OptionListUI* pInspector = (OptionListUI*)ImGuiMgr::GetInst()->FindUI("##Option");
			pInspector->OpenOption(OptionType::OUTLINE);
		}

		tree->SetChildClicked(false);
		ImGui::End();		
	}

	int OutlinerUI::RenderUpdate()
	{
		return 0;
	}


	void OutlinerUI::ResetOutliner()
	{
		// Tree Clear
		tree->Clear();
		tree->AddItem("Root", 0);

		// ���ҽ� �Ŵ������� ���� ��� ���ҽ� ��� �޾ƿ�
		Level* pCurLevel = LevelMgr::GetInst()->GetCurLevel();

		for (UINT i = 0; i < (UINT)MAX_LAYER; ++i)
		{
			Layer* pLayer = pCurLevel->GetLayer(i);

			const vector<GameObject*>& vecParentObj = pLayer->GetParentObject();

			for (size_t i = 0; i < vecParentObj.size(); ++i)
			{
				AddGameObject(vecParentObj[i], nullptr);
			}
		}
	}

	void OutlinerUI::SetTargetToInspector(DWORD_PTR _selected_node)
	{
		TreeNode* pSelectedNode = (TreeNode*)_selected_node;
		GameObject* pSelectObject = (GameObject*)pSelectedNode->GetData();

		// Inspector �� ���õ� GameObject �� �˷��ش�.	
		InspectorUI* pInspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
		pInspector->SetTargetObject(pSelectObject);

		EditorObjMgr::GetInst()->SetTransformTarget(pSelectObject);
	}

	void OutlinerUI::OpenOptionList(DWORD_PTR _selected_node)
	{
		TreeNode* pSelectedNode = (TreeNode*)_selected_node;
		GameObject* pSelectObject = (GameObject*)pSelectedNode->GetData();

		OptionListUI* pInspector = (OptionListUI*)ImGuiMgr::GetInst()->FindUI("##Option");
		pInspector->OpenOption(OptionType::OBJECT ,pSelectedNode);
	}


	void OutlinerUI::AddGameObject(GameObject* _obj, TreeNode* _ParentNode)
	{
		// ������Ʈ�� Ʈ���� �ְ�, ������ ��� �ּҸ� �޾Ƶд�.
		TreeNode* node = tree->AddItem(string(_obj->GetName().begin(), _obj->GetName().end())
			, (DWORD_PTR)_obj
			, _ParentNode);

		// ������Ʈ�� �ڽĿ�����Ʈ ���� ������Ʈ ��带 �θ�� �ؼ� �� ������ �ٽ� �־��ش�.
		const vector<GameObject*>& vecChild = _obj->GetChild();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			AddGameObject(vecChild[i], node);
		}
	}


	GameObject* OutlinerUI::GetSelectedObject()
	{
		TreeNode* pSelectedNode = tree->GetSelectedNode();

		if (nullptr == pSelectedNode)
			return nullptr;

		return (GameObject*)pSelectedNode->GetData();
	}


	void OutlinerUI::DragDrop(DWORD_PTR _DragNode, DWORD_PTR _DropNode)
	{
		TreeNode* pDragNode = (TreeNode*)_DragNode;
		TreeNode* pDropNode = (TreeNode*)_DropNode;

		GameObject* pDragObj = (GameObject*)pDragNode->GetData();
		GameObject* pDropObj = nullptr;
		if (nullptr != pDropNode)
		{
			pDropObj = (GameObject*)pDropNode->GetData();
		}

		// �ڽ����� �� ������Ʈ�� ������ ������Ʈ�� ����(�θ����) �� �ϳ����, 
		// AddChild ó������ �ʴ´�.
		if (nullptr != pDropObj)
		{
			if (pDropObj->IsAncestor(pDragObj))
				return;
		}

		// �̺�Ʈ �Ŵ����� ���ؼ� ó���Ѵ�.
		Event evn = {};
		evn.type = EVENT_TYPE::ADD_CHILD;
		evn.w_param = (DWORD_PTR)pDropObj;
		evn.l_param = (DWORD_PTR)pDragObj;
		EventMgr::GetInst()->AddEvent(evn);
	}
}