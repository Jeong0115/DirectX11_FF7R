#pragma once
#include "UI.h"

namespace ff7r
{
    class TreeNode;

    enum class OptionType
    {
        OBJECT,
        RESOURCE,
        OUTLINE,
        CONTENT
    };

    class OptionListUI : public UI
    {
    public:
        OptionListUI();
        virtual ~OptionListUI();

        void OpenOption(OptionType _type, TreeNode* _target = nullptr);

        virtual void FinalTick() override;
        virtual int RenderUpdate();

        void Close();
    private:
        void UpdateObjectType();
        void UpdateResType();
        void UpdateOutline();
        void UpdateContent();

        void CreateEmptyObj();
        void CreateMaterial();

        void ReName();
        void Delte();

        GameObject* copy_obj;
        Resource*   copy_res;
        TreeNode*   target_node;
        OptionType  type;

        bool        is_first;

    };
}
