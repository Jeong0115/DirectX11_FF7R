#pragma once
#include "UI.h"
namespace ff7r
{
    class TreeUI;
    class TreeNode;

    class OutlinerUI : public UI
    {
    public:
        OutlinerUI();
        ~OutlinerUI();

        virtual void Tick() override;
        virtual void FinalTick() override;
        virtual int RenderUpdate() override;
        void ResetOutliner();
        void SetTargetToInspector(DWORD_PTR _selected_node);
        void OpenOptionList(DWORD_PTR _selected_node);

        GameObject* GetSelectedObject();

        // Ʈ���� ���µ� ���, �Է����� ���� �����Ϳ� ������ ��带  ���û��·� �д�.
        void SetSelectedNodeData(DWORD_PTR _data) { m_dwSelectedData = _data; }

    private:
        void AddGameObject(GameObject* _Obj, TreeNode* _ParentNode);
        void DragDrop(DWORD_PTR _DragNode, DWORD_PTR _DropNode);

        TreeUI*     tree;
        DWORD_PTR   m_dwSelectedData;
    };
}