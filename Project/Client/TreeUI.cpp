#include "pch.h"

#include <Engine/GameObject.h>
#include <Engine/TimeMgr.h>

#include "ImGuiEventMgr.h"

#include "TreeUI.h"
namespace ff7r
{

    // ========
    // TreeNode
    // ========
    TreeNode::TreeNode()
        : owner(nullptr)
        , parent_node(nullptr)
        , Id(0)
        , data(0)
        , m_CategoryNode(false)
        , m_Hilight(false)
    {
    }

    TreeNode::~TreeNode()
    {
        Safe_Del_Vec(child_node);
    }


    void TreeNode::render_update()
    {

        // Flag 체크
        UINT flag = ImGuiTreeNodeFlags_DefaultOpen;

        // 자식 노드가 없으면 Lear 플래그를 설정한다(화살표 제거)
        if (child_node.empty())
            flag |= ImGuiTreeNodeFlags_Leaf;

        // 클릭 되었거나, 항목 대표 노드인 경우 Selected 플래그로 하이라이트를 준다.
        if (m_Hilight || m_CategoryNode)
            flag |= ImGuiTreeNodeFlags_Selected;


        if (ImGui::TreeNodeEx(name.c_str(), flag))
        {
            if (owner->m_SelectedNode == this && ImGui::IsKeyPressed(ImGuiKey_F2))
            {
                Rename();
            }
            if (change_name)
            {
                InputText();
            }
            if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Left))
            {
                if (owner->m_SelectedNode == this)
                {
                    owner->SetDoubleClickNode(this);
                }
                else
                {
                    owner->m_LbtDownNode = this;
                }
            }
            else if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left))
            {
                owner->SetSelectedNode(this);
            }
            // 또는, 트리 재구성 이전에 선택된 노드였다면, 다시 선택노드로 처리한다.
            else if (0 != owner->m_dwPrevSelected && owner->m_dwPrevSelected == data)
            {
                owner->m_dwPrevSelected = 0;
                owner->SetSelectedNode(this);
            }

            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1))
            {
                owner->m_LbtDownNode = this;
                owner->SetSelectedNode(this);
                owner->CallOptionList(this);
            }

            // 해당 노드 위에서 드래그 스타트 체크
            if (ImGui::BeginDragDropSource())
            {
                ImGui::SetDragDropPayload(owner->m_strDragDropID.c_str(), this, sizeof(TreeNode));
                ImGui::Text(name.c_str());

                // Tree 에 드래그 노드 등록
                owner->SetDragNode(this);

                ImGui::EndDragDropSource();
            }


            // 드래그 시작 후, 드랍의 후보인 경우
            if (ImGui::BeginDragDropTarget())
            {
                // 해당 노드에서 마우스 뗀 경우, 지정한 PayLoad 키값이 일치한 경우
                const ImGuiPayload* pPayLoad = ImGui::AcceptDragDropPayload(owner->m_strDragDropID.c_str());
                if (pPayLoad)
                {
                    // DropNode 를 Tree 에 알림
                    owner->SetDropNode(this);
                }

                ImGui::EndDragDropTarget();
            }

            for (size_t i = 0; i < child_node.size(); ++i)
            {
                child_node[i]->render_update();
            }

            ImGui::TreePop();
        }

    }

    void TreeNode::InputText()
    {
		if (first_input_name)
		{
            CopyStrToChar(name, name_buff, 50);
			first_input_name = false;

			ImGui::SetKeyboardFocusHere();
			ImGui::InputText("##Input New Name", name_buff, IM_ARRAYSIZE(name_buff), ImGuiInputTextFlags_AutoSelectAll);

		}
		else if (!first_input_name)
		{
			ImGui::InputText("##Input New Name", name_buff, IM_ARRAYSIZE(name_buff), ImGuiInputTextFlags_AutoSelectAll);
			if (!ImGui::IsItemActive() && !ImGui::IsItemHovered()) 
            {
                string new_name(name_buff);
                if (new_name == "")
                {
                    new_name = "Default";
                }

                ImguiEvent even;
                even.target = data;
                even.desc = new_name;
                even.event_type = IMGUI_EVENT_TYPE::RENAME;

                ImGuiEventMgr::GetInst()->AddEvent(even);
				
                change_name = false;
			}
		}
    }



    // ================
    //      TreeUI
    // ================
    TreeUI::TreeUI()
        : UI("##Tree")
        , root_node(nullptr)
        , Next_Id(0)
        , m_bShowRoot(true)
        , m_SelectedNode(nullptr)
        , m_dwPrevSelected(0)
        , m_SelectInst(nullptr)
        , m_SelectFunc(nullptr)
        , m_DragDropInst(nullptr)
        , m_DragDropFunc(nullptr)
        , m_DoubleClickDelay(0.0f)
        , m_DoubleClikInst(nullptr)
        , m_DoubleClikFunc(nullptr)
        , root_ui(nullptr)
        , open_option_func(nullptr)
    {
        m_strDragDropID = "PayLoad";
    }

    TreeUI::~TreeUI()
    {
        if (nullptr != root_node)
            delete root_node;
    }
    
    int TreeUI::RenderUpdate()
    {
        if (nullptr != root_node)
        {
            if (m_bShowRoot)
            {
                root_node->render_update();
            }
            else
            {
                for (size_t i = 0; i < root_node->child_node.size(); ++i)
                {
                    root_node->child_node[i]->render_update();
                }
            }
        }

        // Drag Drop 노드 둘다 있는 경우
        if ((m_DragNode && m_DropNode) || (m_DragNode && ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left)))
        {
            if (m_DragDropInst && m_DragDropFunc)
            {
                (m_DragDropInst->*m_DragDropFunc)((DWORD_PTR)m_DragNode, (DWORD_PTR)m_DropNode);
            }

            m_DragNode = nullptr;
            m_DropNode = nullptr;
        }

        return 0;
    }

    void TreeUI::Clear()
    {
        // 이전에 선택된 노드가 있으면
        if (nullptr != m_SelectedNode)
        {
            // 이름을 기억해둔다.
            m_dwPrevSelected = m_SelectedNode->GetData();
        }

        if (nullptr != root_node)
        {
            delete root_node;
            root_node = nullptr;
        }
    }

    TreeNode* TreeUI::AddItem(const string& _node_name, DWORD_PTR _data, TreeNode* _parent)
    {
        TreeNode* new_node = new TreeNode;

        new_node->owner = this;
        new_node->name = _node_name;
        new_node->data = _data;
        new_node->Id = Next_Id++;

        // 루트가 NULL 이다 ==> 트리에 들어온 최초의 데이터
        if (nullptr == root_node)
        {
            // 최초 데이터 입력인데, 부모를 지정한 경우
            assert(!_parent);

            root_node = new_node;
        }

        // 트리에 들어온 데이터가 최초가 아니다.
        else
        {
            if (_parent)
            {
                // 노드의 부모로 지정된 노드가 해당 트리 소속이 아니다.
                if (_parent->owner != this)
                    assert(nullptr);

                // 지정된 부모의 자식으로 연결
                _parent->child_node.push_back(new_node);
                new_node->parent_node = _parent;
            }

            // 부모로 지정된 노드가 없는경우, 루트 밑으로 넣는다
            else
            {
                // 새로 생성한 노드를 루트노드의 자식으로 연결
                root_node->child_node.push_back(new_node);
                new_node->parent_node = root_node;
            }
        }

        return new_node;
    }


    void TreeUI::SetSelectedNode(TreeNode* _Node)
    {
        // 마우스를 누른 노드와 뗀 노드가 일치해야 함
        if (m_LbtDownNode != _Node)
        {
            m_LbtDownNode = nullptr;
            return;
        }

        if (m_SelectedNode)
            m_SelectedNode->m_Hilight = false;

        m_SelectedNode = _Node;
        m_LbtDownNode = nullptr;

        if (m_SelectedNode)
        {
            m_SelectedNode->m_Hilight = true;

            if (m_SelectInst && m_SelectFunc)
            {
                (m_SelectInst->*m_SelectFunc)((DWORD_PTR)m_SelectedNode);
            }
        }
    }

    void TreeUI::SetDoubleClickNode(TreeNode* _Node)
    {
        if (m_SelectedNode)
            m_SelectedNode->m_Hilight = false;

        m_SelectedNode = _Node;
        m_LbtDownNode = nullptr;

        if (m_SelectedNode)
        {
            m_SelectedNode->m_Hilight = true;

            if (m_DoubleClikInst && m_DoubleClikFunc)
            {
                (m_DoubleClikInst->*m_DoubleClikFunc)((DWORD_PTR)m_SelectedNode);
            }
        }
    }

    void TreeUI::CallOptionList(TreeNode* _Node)
    {
        if (!_Node) return;

        if (root_ui && open_option_func && !child_clicked)
        {
            (root_ui->*open_option_func)((DWORD_PTR)_Node);
            child_clicked = true;
        }
    }

    void TreeUI::SetDragNode(TreeNode* _Node)
    {
        m_DragNode = _Node;
        m_DropNode = nullptr;
    }

    void TreeUI::SetDropNode(TreeNode* _Node)
    {
        m_DropNode = _Node;
    }

    bool TreeUI::GetSelectedNode(DWORD_PTR _Data)
    {
        static list<TreeNode*> queue;
        queue.clear();

        queue.push_back(root_node);

        TreeNode* pCurNode = nullptr;
        while (!queue.empty())
        {
            pCurNode = queue.front();
            queue.pop_front();

            for (size_t i = 0; i < pCurNode->child_node.size(); ++i)
            {
                queue.push_back(pCurNode->child_node[i]);
            }

            if (pCurNode->GetData() == _Data)
            {
                SetSelectedNode(pCurNode);
                return true;
            }
        }

        return false;
    }
}