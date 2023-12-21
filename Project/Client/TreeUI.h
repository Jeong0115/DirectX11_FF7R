#pragma once
#include "UI.h"
namespace ff7r
{


    typedef void (UI::*ADD_EVENT)(ImguiEvent);

    // =========
    // TreeNode     
    // =========
    class TreeUI;
    class TreeNode
    {
    private:
        TreeUI* owner;        // 노드를 소유하고 있는 트리
        TreeNode* parent_node;   // 부모노드
        vector<TreeNode*>   child_node; // 노드의 자식 노드

        string              name;      // 노드의 출력 이름
        UINT                Id;           // 노드의 고유 ID

        DWORD_PTR           data;         // 노드에 저장된 데이터


        bool                m_CategoryNode; // 항목 대표 노드
        bool                m_Hilight;      // 노드 하이라이트 처리
        bool                change_name;

    public:
        void Rename() { first_input_name = change_name = true; }
        void SetCategoryNode(bool _category)
        {
            m_CategoryNode = _category;
        }

    public:
        const string& GetName() { return name; }
        DWORD_PTR GetData() { return data; }

    private:
        void render_update();
        void InputText();

        char name_buff[50];
        bool first_input_name;
    public:
        TreeNode();
        ~TreeNode();

        friend class TreeUI;
    };



    // ======
    // TreeUI
    // ======



    class TreeUI : public UI
    {
    private:
        TreeNode* root_node; // 트리가 소유하고 있는 노드 중 루트 노드
        UINT            Next_Id;   // 생성되는 노드뒤에 붙여줄 고유 숫자
        bool            m_bShowRoot;
        bool child_clicked;

        TreeNode* m_SelectedNode;
        TreeNode* m_LbtDownNode;

        TreeNode* m_DragNode;
        TreeNode* m_DropNode;

        DWORD_PTR       m_dwPrevSelected;

        UI* m_SelectInst;
        UI_DELEGATE_1   m_SelectFunc;

        UI* m_DoubleClikInst;
        UI_DELEGATE_1   m_DoubleClikFunc;

        UI* root_ui;
        UI_DELEGATE_1   open_option_func;

        UI* m_DragDropInst;
        UI_DELEGATE_2   m_DragDropFunc;

        string          m_strDragDropID;

        float           m_DoubleClickDelay;
    public:
        virtual int RenderUpdate() override;
        bool IsChildClicked() { return child_clicked; }
        void SetChildClicked(bool is_clicked) { child_clicked = is_clicked; }
    public:
        void Clear();

        TreeNode* AddItem(const string& _strNodeName, DWORD_PTR _Data, TreeNode* _pParent = nullptr);
        void ShowRoot(bool _Show) { m_bShowRoot = _Show; }

        void AddDynamic_Select(UI* _ui, UI_DELEGATE_1 _MemFunc)
        {
            m_SelectInst = _ui;
            m_SelectFunc = _MemFunc;
        }

        void AddDynamic_DoubleClik(UI* _ui, UI_DELEGATE_1 _MemFunc)
        {
            m_DoubleClikInst = _ui;
            m_DoubleClikFunc = _MemFunc;
        }

        void AddDynamic_Option(UI* _ui, UI_DELEGATE_1 _open_option_func)
        {
            root_ui = _ui;
            open_option_func = _open_option_func;
        }
        void AddDynamic_DragDrop(UI* _ui, UI_DELEGATE_2 _MemFunc)
        {
            m_DragDropInst = _ui;
            m_DragDropFunc = _MemFunc;
        }

        void SetDragDropID(const string& _strID) { m_strDragDropID = _strID; }

        void AddEvent(ImguiEvent _event);

    private:
        void SetSelectedNode(TreeNode* _Node);
        void SetDoubleClickNode(TreeNode* _Node);
        void CallOptionList(TreeNode* _Node);
        void SetDragNode(TreeNode* _Node);
        void SetDropNode(TreeNode* _Node);

    public:
        TreeNode* GetSelectedNode() { return m_SelectedNode; }
        bool GetSelectedNode(DWORD_PTR _Data);  // 입력된 데이터와 동일한 데이터를 보유하고 있는 노드를 선택상태로 만듬


    public:
        TreeUI();
        ~TreeUI();

        friend class TreeNode;
    };

}