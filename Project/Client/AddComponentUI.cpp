#include "pch.h"

#include "AddComponentUI.h"
#include <Script\ScriptMgr.h>
namespace ff7r
{
    AddComponentUI::AddComponentUI()
        : UI("##AddComp")
        , mCompList(nullptr)
    {
        SetName("AddComponent");
    }

    AddComponentUI::~AddComponentUI()
    {
    }

    void AddComponentUI::Init()
    {
        SetActive(true);
    }

    void AddComponentUI::FinalTick()
    {
        if (!IsActive())
            return;

        RenderUpdate();
    }

    int AddComponentUI::RenderUpdate()
    {
        if (ImGui::BeginChild("Search"))
        {

            for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
            {
                ImGui::Button(ToString((COMPONENT_TYPE)i));
            }

            vector<wstring> vecScripts;
            ScriptMgr::GetScriptInfo(vecScripts);

            for (size_t i = 0; i < vecScripts.size(); ++i)
            {
                string strScriptName = string(vecScripts[i].begin(), vecScripts[i].end());
                ImGui::Button(strScriptName.c_str());
            }

            ImGui::EndChild();
        }

        return 1;
    }
}