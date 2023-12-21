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

        // Flag üũ
        UINT flag = ImGuiTreeNodeFlags_DefaultOpen;

        // �ڽ� ��尡 ������ Lear �÷��׸� �����Ѵ�(ȭ��ǥ ����)
        if (child_node.empty())
            flag |= ImGuiTreeNodeFlags_Leaf;

        // Ŭ�� �Ǿ��ų�, �׸� ��ǥ ����� ��� Selected �÷��׷� ���̶���Ʈ�� �ش�.
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
            // �Ǵ�, Ʈ�� �籸�� ������ ���õ� ��忴�ٸ�, �ٽ� ���ó��� ó���Ѵ�.
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

            // �ش� ��� ������ �巡�� ��ŸƮ üũ
            if (ImGui::BeginDragDropSource())
            {
                ImGui::SetDragDropPayload(owner->m_strDragDropID.c_str(), this, sizeof(TreeNode));
                ImGui::Text(name.c_str());

                // Tree �� �巡�� ��� ���
                owner->SetDragNode(this);

                ImGui::EndDragDropSource();
            }


            // �巡�� ���� ��, ����� �ĺ��� ���
            if (ImGui::BeginDragDropTarget())
            {
                // �ش� ��忡�� ���콺 �� ���, ������ PayLoad Ű���� ��ġ�� ���
                const ImGuiPayload* pPayLoad = ImGui::AcceptDragDropPayload(owner->m_strDragDropID.c_str());
                if (pPayLoad)
                {
                    // DropNode �� Tree �� �˸�
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

        // Drag Drop ��� �Ѵ� �ִ� ���
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
        // ������ ���õ� ��尡 ������
        if (nullptr != m_SelectedNode)
        {
            // �̸��� ����صд�.
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

        // ��Ʈ�� NULL �̴� ==> Ʈ���� ���� ������ ������
        if (nullptr == root_node)
        {
            // ���� ������ �Է��ε�, �θ� ������ ���
            assert(!_parent);

            root_node = new_node;
        }

        // Ʈ���� ���� �����Ͱ� ���ʰ� �ƴϴ�.
        else
        {
            if (_parent)
            {
                // ����� �θ�� ������ ��尡 �ش� Ʈ�� �Ҽ��� �ƴϴ�.
                if (_parent->owner != this)
                    assert(nullptr);

                // ������ �θ��� �ڽ����� ����
                _parent->child_node.push_back(new_node);
                new_node->parent_node = _parent;
            }

            // �θ�� ������ ��尡 ���°��, ��Ʈ ������ �ִ´�
            else
            {
                // ���� ������ ��带 ��Ʈ����� �ڽ����� ����
                root_node->child_node.push_back(new_node);
                new_node->parent_node = root_node;
            }
        }

        return new_node;
    }


    void TreeUI::SetSelectedNode(TreeNode* _Node)
    {
        // ���콺�� ���� ���� �� ��尡 ��ġ�ؾ� ��
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