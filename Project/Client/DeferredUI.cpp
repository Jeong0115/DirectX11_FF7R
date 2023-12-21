#include "pch.h"

#include <Engine/ResourceMgr.h>
#include <Engine/Texture.h>

#include "DeferredUI.h"

namespace ff7r
{
	DeferredUI::DeferredUI()
        : UI("##Deferred")
        , diffuse_tex{}
        , normal_tex{}
        , position_tex{}
        , data_tex{}
	{
        SetName("Deferred");
	}

	DeferredUI::~DeferredUI()
	{
	}

    void DeferredUI::Init()
    {
        diffuse_tex = ResourceMgr::GetInst()->FindRes<Texture>(L"DiffuseTargetTex");
        normal_tex = ResourceMgr::GetInst()->FindRes<Texture>(L"NormalTargetTex");
        position_tex = ResourceMgr::GetInst()->FindRes<Texture>(L"PositionTargetTex");
        data_tex = ResourceMgr::GetInst()->FindRes<Texture>(L"DataTargetTex");
    }

    int DeferredUI::RenderUpdate()
	{
        ImVec2 uv_min = ImVec2(0.0f, 0.0f);
        ImVec2 uv_max = ImVec2(1.0f, 1.0f);
        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);

        if (diffuse_tex != nullptr)
        {
            ImGui::Text("Diffuse");
            ImGui::Image((ImTextureID)diffuse_tex->GetSRV().Get(), ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);
        }

        if (normal_tex != nullptr)
        {
            ImGui::Text("Normal");
            ImGui::Image((ImTextureID)normal_tex->GetSRV().Get(), ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);
        }

        if (position_tex != nullptr)
        {
            ImGui::Text("Position");
            ImGui::Image((ImTextureID)position_tex->GetSRV().Get(), ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);
        }

        if (data_tex != nullptr)
        {
            ImGui::Text("Data");
            ImGui::Image((ImTextureID)data_tex->GetSRV().Get(), ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);
        }
		return 1;
	}
}