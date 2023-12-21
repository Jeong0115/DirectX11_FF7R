#include "pch.h"

#include <Engine/GameObject.h>
#include <Engine/Resource.h>
#include <Engine/EventMgr.h>
#include <Engine/ResourceMgr.h>
#include <Engine/PathMgr.h>

#include "EditorObjMgr.h"
#include "ContentUI.h"
#include "ImGuiEventMgr.h"
#include "InspectorUI.h"
#include "OutlinerUI.h"
#include "TreeUI.h"

#include "OptionListUI.h"
namespace ff7r
{
    OptionListUI::OptionListUI()
        : UI("##Option")
        , type(OptionType::OBJECT)
        , copy_res(nullptr)
        , copy_obj(nullptr)
        , target_node(nullptr)
        , is_first(false)
    {
        SetName("Option");
    }

    OptionListUI::~OptionListUI()
    {
    }

    void OptionListUI::OpenOption(OptionType _type, TreeNode* _target)
    {
        SetActive(true);
        target_node = _target;
        type = _type;
        is_first = true;
    }



    void OptionListUI::FinalTick()
    {
        if (!IsActive())
            return;

        //ImGui::OpenPopup("##Object Option");
        RenderUpdate();
    }

    int OptionListUI::RenderUpdate()
	{
        if (is_first)
        {
            is_first = false;
            ImGui::OpenPopup("##Object Option");
        }
    
		if (ImGui::BeginPopup("##Object Option"))
		{
			switch (type)
            {
            case OptionType::OBJECT:
                UpdateObjectType();
                break;
            
            case OptionType::RESOURCE:
                UpdateResType();
                break;

            case OptionType::OUTLINE:
                UpdateOutline();
                break;

            case OptionType::CONTENT:
                UpdateContent();
                break;
            
            default: Close(); break;
            }

            ImGui::EndPopup();
        }
        else
        {
            Close();
        }

        return 1;
    }

    void OptionListUI::Close()
    {
        SetActive(false);
        target_node = nullptr;
        ImGui::CloseCurrentPopup();
        //mCopyObject = nullptr;
        //mCopyRes = nullptr;
    }

    void OptionListUI::UpdateObjectType()
    {
        if (ImGui::Selectable("Copy"))
        {
            copy_obj = (GameObject*)target_node->GetData();
            Close();
        }
        if (ImGui::Selectable("Paste"))
        {
            if (copy_obj)
            {
                Event instantiate;
                instantiate.type = EVENT_TYPE::CREATE_OBJECT;
                instantiate.w_param = (DWORD_PTR)copy_obj->Clone();
                instantiate.l_param = (DWORD_PTR)copy_obj->GetLayerIndex();

                EventMgr::GetInst()->AddEvent(instantiate);

                copy_obj = nullptr;
            }
            Close();
        }
		if (ImGui::Selectable("Rename"))
		{
			target_node->Rename();
		}
        if (ImGui::Selectable("Delete"))
        {
            GameObject* target = (GameObject*)target_node->GetData();
            if (target)
            {
                Event instantiate;
                instantiate.type = EVENT_TYPE::DELETE_OBJECT;
                instantiate.w_param = (DWORD_PTR)target;

                EventMgr::GetInst()->AddEvent(instantiate);

                InspectorUI* inspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
                if (inspector->GetTargetObject() == target)
                {
                    inspector->SetTargetObject(nullptr);
                }
                EditorObjMgr::GetInst()->SetTransformTarget(nullptr);

                target = nullptr;
                target_node = nullptr;
            }
            Close();
        }

        if (!ImGui::IsWindowFocused())
        {
            Close();
        }

    }

    void OptionListUI::UpdateResType()
    {
		if (ImGui::Selectable("Rename"))
		{
			target_node->Rename();
		}
        if (ImGui::Selectable("Delete", false, ImGuiSelectableFlags_Disabled))
        {
            Close();
        }
        if (!ImGui::IsWindowFocused())
        {
            Close();
        }

    }

    void OptionListUI::UpdateOutline()
    {
        if (ImGui::Selectable("Copy", false, ImGuiSelectableFlags_Disabled))
        {
        }
        if (ImGui::Selectable("Paste"))
        {
            if (copy_obj)
            {
                Event instantiate;
                instantiate.type = EVENT_TYPE::CREATE_OBJECT;
                instantiate.w_param = (DWORD_PTR)copy_obj->Clone();
                instantiate.l_param = (DWORD_PTR)copy_obj->GetLayerIndex();

                EventMgr::GetInst()->AddEvent(instantiate);

                copy_obj = nullptr;
            }
            Close();
        }
        if (ImGui::Selectable("Rename", false, ImGuiSelectableFlags_Disabled))
        {
        }
        if (ImGui::Selectable("Delete", false, ImGuiSelectableFlags_Disabled))
        {
        }
        if (ImGui::Selectable("Create Empty"))
        {
            ImguiEvent even;
            even.event_type = IMGUI_EVENT_TYPE::CREATE;
            even.res_type = RES_TYPE::END;

            ImGuiEventMgr::GetInst()->AddEvent(even);
            Close();
        }
        if (ImGui::Selectable("Create UI"))
        {
            ImguiEvent even;
            even.event_type = IMGUI_EVENT_TYPE::CREATE;
            even.res_type = RES_TYPE::END;
            even.desc = "UI";

            ImGuiEventMgr::GetInst()->AddEvent(even);
            Close();
        }
        if (!ImGui::IsWindowFocused())
        {
            Close();
        }
    }
    void OptionListUI::UpdateContent()
    {
        if (ImGui::Selectable("Rename", false, ImGuiSelectableFlags_Disabled))
        {
            target_node->Rename();
        }
        if (ImGui::Selectable("Delete", false, ImGuiSelectableFlags_Disabled))
        {
            Close();
        }
        if (ImGui::Selectable("Create Material"))
        {
            ImguiEvent even;
            even.event_type = IMGUI_EVENT_TYPE::CREATE;
            even.res_type = RES_TYPE::MATERIAL;

            ImGuiEventMgr::GetInst()->AddEvent(even);
            Close();
        }
    }

    void OptionListUI::CreateEmptyObj()
    {

    }

    void OptionListUI::CreateMaterial()
    {

    }
    void OptionListUI::ReName()
    {
    }
    void OptionListUI::Delte()
    {
    }
}
