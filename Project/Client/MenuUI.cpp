#include "pch.h"
#include "MenuUI.h"

#include <Engine\EventMgr.h>
#include <Engine\GameObject.h>
#include <Engine\components.h>
#include <Engine\Script.h>
#include <Engine\LevelMgr.h>

#include <Script\ScriptMgr.h>



#include "ImGuiMgr.h"
#include "OutlinerUI.h"
#include "InspectorUI.h"
#include "LevelSaveLoad.h"


namespace ff7r
{
    MenuUI::MenuUI()
        : UI("##Menu")
    {
        SetName("Menu");
    }

    MenuUI::~MenuUI()
    {
    }

    void MenuUI::FinalTick()
    {
        if (!IsActive())
            return;

        RenderUpdate();
    }

    int MenuUI::RenderUpdate()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Save Level"))
                {
                    // Level 저장
                    wstring levelName = LevelMgr::GetInst()->GetCurLevel()->GetName();

                    LevelSaveLoad::SaveLevel(levelName, LevelMgr::GetInst()->GetCurLevel());
                }

                if (ImGui::MenuItem("Load Level"))
                {
                    // Level 불러오기
                    Level* pLoadedLevel = LevelSaveLoad::LoadLevel(L"Level\\TestLevel.lv");

                    Event evn = {};
                    evn.type = EVENT_TYPE::LEVEL_CHANGE;
                    evn.w_param = (DWORD_PTR)pLoadedLevel;

                    EventMgr::GetInst()->AddEvent(evn);
                }

                if (ImGui::MenuItem("New Level"))
                {
                    // Level 불러오기
                    //CLevel* pLoadedLevel = new CLevel;
                    //CLevelSaveLoad::SaveLevel(L"Level\\NewLevel.lv", pLoadedLevel);

                    ResourceMgr::GetInst()->CreateNewResource(L"NewLevel2", RES_TYPE::LEVEL);
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Level"))
            {
                Level* CurLevel = LevelMgr::GetInst()->GetCurLevel();
                bool PlayEnable = true;
                bool PauseEnable = true;
                bool StopEnable = true;

                if (CurLevel->GetState() == LEVEL_STATE::PLAY)
                {
                    PlayEnable = false;
                    PauseEnable = true;
                    StopEnable = true;
                }
                else if (CurLevel->GetState() == LEVEL_STATE::PAUSE)
                {
                    PlayEnable = true;
                    PauseEnable = false;
                    StopEnable = true;
                }
                else if (CurLevel->GetState() == LEVEL_STATE::STOP)
                {
                    PlayEnable = true;
                    PauseEnable = false;
                    StopEnable = false;
                }



                if (ImGui::MenuItem("Play", nullptr, nullptr, PlayEnable))
                {
                    LevelSaveLoad::SaveLevel(L"Level\\Temp.lv", CurLevel);
                    CurLevel->ChangeState(LEVEL_STATE::PLAY);
                }
                else if (ImGui::MenuItem("Pause", nullptr, nullptr, PauseEnable))
                {
                    CurLevel->ChangeState(LEVEL_STATE::PAUSE);
                }
                else if (ImGui::MenuItem("Stop", nullptr, nullptr, StopEnable))
                {
                    CurLevel->ChangeState(LEVEL_STATE::STOP);
                    Level* pNewLevel = LevelSaveLoad::LoadLevel(L"Level\\Temp.lv");

                    Event evn = {};
                    evn.type = EVENT_TYPE::LEVEL_CHANGE;
                    evn.w_param = DWORD_PTR(pNewLevel);
                    EventMgr::GetInst()->AddEvent(evn);

                    // InspectorUI
                    InspectorUI* Inspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
                    Inspector->SetTargetObject(nullptr);
                }

                ImGui::EndMenu();
            }




            ImGui::EndMainMenuBar();
        }

        return 1;
    }


  // void MenuUI::CreateEmptyMaterial(const wstring& _strMtrlName)
  // {
  //     Ptr<Material> pNewMtrl = new Material;
  //     ResourceMgr::GetInst()->AddRes<Material>(L"material\\" + _strMtrlName + L".mtrl", pNewMtrl);
  //     pNewMtrl->SetName(_strMtrlName);
  //     pNewMtrl->Save(pNewMtrl->GetKey());
  // }
}