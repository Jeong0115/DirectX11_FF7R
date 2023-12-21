#include "pch.h"
#include "ScriptUI.h"

#include <Script\ScriptMgr.h>

#include "ParamUI.h"
namespace ff7r
{
	ScriptUI::ScriptUI()
		: ComponentUI("##Script", COMPONENT_TYPE::SCRIPT)
		, m_pTargetScript(nullptr)
	{

	}

	ScriptUI::~ScriptUI()
	{

	}

	int ScriptUI::RenderUpdate()
	{
		if (nullptr == GetTarget())
			return 0;

		ImGui::PushID(0);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));

		ImGui::Button(m_strScriptName.c_str());

		ImGui::PopStyleColor(3);
		ImGui::PopID();

		ImGui::NewLine();

		// Script 에서 요구하는 ScalarParameter 를 UI 에 노출	
		const vector<ScriptParam>& vecScriptParam = m_pTargetScript->GetScritpParam();
		if (vecScriptParam.empty())
			return 1;

		ImGui::Text("Parameter");

		for (size_t i = 0; i < vecScriptParam.size(); ++i)
		{
			switch (vecScriptParam[i].param)
			{
			case SCRIPT_PARAM::INT:
				ParamUI::Param_Int(vecScriptParam[i].desc, (int*)vecScriptParam[i].data);
				break;
			case SCRIPT_PARAM::FLOAT:
				ParamUI::Param_Float(vecScriptParam[i].desc, (float*)vecScriptParam[i].data);
				break;
			case SCRIPT_PARAM::VEC2:
				ParamUI::Param_Vec2(vecScriptParam[i].desc, (vec2*)vecScriptParam[i].data);
				break;
			case SCRIPT_PARAM::VEC4:
				ParamUI::Param_Vec4(vecScriptParam[i].desc, (vec4*)vecScriptParam[i].data);
				break;
			default:
				break;
			}
		}


		return 1;
	}

	void ScriptUI::SetScript(Script* _Script)
	{
		assert(_Script);
		m_pTargetScript = _Script;
		wstring wstr = ScriptMgr::GetScriptName(m_pTargetScript);
		m_strScriptName = string(wstr.begin(), wstr.end());
	}
}