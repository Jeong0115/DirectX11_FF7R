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
        TreeUI* owner;        // ��带 �����ϰ� �ִ� Ʈ��
        TreeNode* parent_node;   // �θ���
        vector<TreeNode*>   child_node; // ����� �ڽ� ���

        string              name;      // ����� ��� �̸�
        UINT                Id;           // ����� ���� ID

        DWORD_PTR           data;         // ��忡 ����� ������


        bool                m_CategoryNode; // �׸� ��ǥ ���
        bool                m_Hilight;      // ��� ���̶���Ʈ ó��
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
        TreeNode* root_node; // Ʈ���� �����ϰ� �ִ� ��� �� ��Ʈ ���
        UINT            Next_Id;   // �����Ǵ� ���ڿ� �ٿ��� ���� ����
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
        bool GetSelectedNode(DWORD_PTR _Data);  // �Էµ� �����Ϳ� ������ �����͸� �����ϰ� �ִ� ��带 ���û��·� ����


    public:
        TreeUI();
        ~TreeUI();

        friend class TreeNode;
    };

}