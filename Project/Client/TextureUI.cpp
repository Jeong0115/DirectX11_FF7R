#include "pch.h"

#include <Engine/Texture.h>

#include "TextureUI.h"
namespace ff7r
{
    TextureUI::TextureUI()
        : ResourceUI(RES_TYPE::TEXTURE)
    {
        SetName("Texture");
    }

    TextureUI::~TextureUI()
    {
    }

    int TextureUI::RenderUpdate()
    {
        ResourceUI::RenderUpdate();

        if (RES_TYPE::TEXTURE == GetTargetRes()->GetType())
        {
            Ptr<Texture> texture = (Texture*)GetTargetRes().Get();

            int width = texture->GetWidth();
            int height = texture->GetHeight();

            ImGui::Text("Width");
            ImGui::SameLine();
            ImGui::InputInt("##Width", &width, 50, ImGuiInputTextFlags_ReadOnly);

            ImGui::Text("Height");
            ImGui::SameLine();
            ImGui::InputInt("##Height", &height, 50, ImGuiInputTextFlags_ReadOnly);

            ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
            ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
            ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
            ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white

            ImGui::Image((ImTextureID)texture->GetSRV().Get(), ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);

        }
        return 0;
    }
}