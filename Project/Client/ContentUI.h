#pragma once
#include "UI.h"

namespace ff7r
{
    class TreeUI;



    class ContentUI : public UI
    {
    public:
        ContentUI();
        ~ContentUI();

        virtual void Init() override;
        virtual void Tick() override;
        virtual void FinalTick() override;
        virtual int RenderUpdate() override;

        void Reload(); // Content ������ �ִ� ���ҽ��� �ε�
        void ResetContent();
        void SetTargetToInspector(DWORD_PTR _SelectedNode);
        void DoubleClikContent(DWORD_PTR _SelectedNode);
        void OpenOptionList(DWORD_PTR _SelectedNode);

    private:
        void FindFileName(const wstring& _FolderPath);
        RES_TYPE GetResTypeByExt(const wstring& _relativepath);

        TreeUI* tree;
        vector<wstring>     res_path;
    };

}