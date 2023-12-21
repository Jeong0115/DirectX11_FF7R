#include "pch.h"
#include "LevelUI.h"

namespace ff7r
{
    LevelUI::LevelUI()
        :ResourceUI(RES_TYPE::LEVEL)
    {
        SetName("Level");
    }

    LevelUI::~LevelUI()
    {
    }

    int LevelUI::RenderUpdate()
    {
        ResourceUI::RenderUpdate();

        //if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Left))
        //{
        //    m_Owner->m_LbtDownNode = this;
        //}
        //else if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left))
        //{
        //    m_Owner->SetSelectedNode(this);
        //}

        return 0;
    }
}