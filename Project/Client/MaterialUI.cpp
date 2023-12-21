#include "pch.h"
#include "MaterialUI.h"

#include "ParamUI.h"
#include "TreeUI.h"

#include <Engine\ResourceMgr.h>
#include <Engine\Material.h>

namespace ff7r
{
    MaterialUI::MaterialUI()
        : ResourceUI(RES_TYPE::MATERIAL)
        , selected_tex(TEX_PARAM::TEX_END)
    {
        SetName("Material");
    }

    MaterialUI::~MaterialUI()
    {
    }

    int MaterialUI::RenderUpdate()
    {
        ResourceUI::RenderUpdate();

        ImGui::Text("Material");
        ImGui::SameLine();

        Ptr<Material> material = (Material*)GetTargetRes().Get();

        // ���߿� ���ҽ��� ������ ��  �� �̸��߰����༭ Ű(���)���� �̸����� ȭ�鿡 �����
        char name_buff[50] = {};
        path res_name = material->GetKey();
        CopyStrToChar(res_name.stem().string(), name_buff, 50);

        ImGui::InputText("##MtrlUIName", name_buff, 50, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

        // GraphicsShader �̸�
        ImGui::Text("Shader  ");
        ImGui::SameLine();

        Ptr<GraphicsShader> shader = material->GetShader();
        if (nullptr != shader)
        {
            res_name = shader->GetKey();
            CopyStrToChar(res_name.stem().string(), name_buff, 50);
            ImGui::InputText("##ShaderUIName", name_buff, 50, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
        }
        else
        {
            char empty[1] = {};
            ImGui::InputText("##ShaderUIName", empty, 1, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
        }

        // GraphicsShader ��� üũ    
        if (ImGui::BeginDragDropTarget())
        {
            // �ش� ��忡�� ���콺 �� ���, ������ PayLoad Ű���� ��ġ�� ���
            const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Resource");
            if (payload)
            {
                TreeNode* node = (TreeNode*)payload->Data;
                Resource* resource = (Resource*)node->GetData();
                if (RES_TYPE::GRAPHICS_SHADER == resource->GetType())
                {
                    ((Material*)GetTargetRes().Get())->SetShader((GraphicsShader*)resource);
                    material->Save(material->GetRelativePath());
                }
            }

            ImGui::EndDragDropTarget();
        }

        ImGui::NewLine();
        ImGui::Text("Parameter");


        // Shader ���� �䱸�ϴ� ScalarParameter �� UI �� ����
        if (nullptr == shader)
            return 0;

        const vector<ScalarParam>& scalar_param = shader->GetScalarParam();

        for (size_t i = 0; i < scalar_param.size(); ++i)
        {
            switch (scalar_param[i].param)
            {
                // Shader Parameter �� Int Ÿ���� ���
            case INT_0:
            case INT_1:
            case INT_2:
            case INT_3:
            {
                // ���� ��Ƽ���� ���õ� ���� ����   
                int data = 0;
                material->GetScalarParam(scalar_param[i].param, &data);
                if (ParamUI::Param_Int(scalar_param[i].desc, &data))
                {
                    // UI �ʿ��� ���� ����Ǿ�����, ���� ��Ƽ���� ���� ����
                    material->SetScalarParam(scalar_param[i].param, &data);
                }
            }
            break;
            case FLOAT_0:
            case FLOAT_1:
            case FLOAT_2:
            case FLOAT_3:
            {
                // ���� ��Ƽ���� ���õ� ���� ����   
                float data = 0;
                material->GetScalarParam(scalar_param[i].param, &data);
                if (ParamUI::Param_Float(scalar_param[i].desc, &data))
                {
                    // UI �ʿ��� ���� ����Ǿ�����, ���� ��Ƽ���� ���� ����
                    material->SetScalarParam(scalar_param[i].param, &data);
                }
            }
            break;
            case VEC2_0:
            case VEC2_1:
            case VEC2_2:
            case VEC2_3:
            {
                // ���� ��Ƽ���� ���õ� ���� ����   
                vec2 data;
                material->GetScalarParam(scalar_param[i].param, &data);
                if (ParamUI::Param_Vec2(scalar_param[i].desc, &data))
                {
                    // UI �ʿ��� ���� ����Ǿ�����, ���� ��Ƽ���� ���� ����
                    material->SetScalarParam(scalar_param[i].param, &data);
                }
            }
            break;
            case VEC4_0:
            case VEC4_1:
            case VEC4_2:
            case VEC4_3:
            {
                // ���� ��Ƽ���� ���õ� ���� ����   
                vec4 data;
                material->GetScalarParam(scalar_param[i].param, &data);
                if (ParamUI::Param_Vec4(scalar_param[i].desc, &data))
                {
                    // UI �ʿ��� ���� ����Ǿ�����, ���� ��Ƽ���� ���� ����
                    material->SetScalarParam(scalar_param[i].param, &data);
                }
            }
            break;
            case MAT_0:
            case MAT_1:
            case MAT_2:
            case MAT_3:
                break;
            }
        }

        // Shader ���� �䱸�ϴ� Texture Parameter �� UI �� ����
        const vector<TexParam>& vecTexParam = shader->GetTexParam();
        for (size_t i = 0; i < vecTexParam.size(); ++i)
        {
            ImGui::NewLine();
            Ptr<Texture> pCurTex = material->GetTexParam(vecTexParam[i].param);
            if (ParamUI::Param_Tex(vecTexParam[i].desc, pCurTex, this, (UI_DELEGATE_1)&MaterialUI::SelectTexture))
            {
                selected_tex = vecTexParam[i].param;
            }
            material->SetTexParam(vecTexParam[i].param, pCurTex);
        }

        return 0;
    }


    void MaterialUI::SelectTexture(DWORD_PTR _Key)
    {
        string strKey = (char*)_Key;
        Ptr<Texture> tex = ResourceMgr::GetInst()->FindRes<Texture>(wstring(strKey.begin(), strKey.end()));

        Ptr<Material> material = (Material*)GetTargetRes().Get();
        material->SetTexParam(selected_tex, tex);
        material->Save(material->GetRelativePath());
    }
}