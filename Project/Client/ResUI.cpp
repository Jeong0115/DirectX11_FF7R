#include "pch.h"
#include "ResUI.h"

namespace ff7r
{
	ResourceUI::ResourceUI(RES_TYPE _type)
		: UI("##ResUI")
		, res_type(_type)
	{

	}

	ResourceUI::~ResourceUI()
	{
	}

	int ResourceUI::RenderUpdate()
	{
		ImGui::PushID(0);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
		ImGui::Button(GetName().c_str());
		ImGui::PopStyleColor(3);
		ImGui::PopID();

		return 0;
	}
}