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

        // 나중에 리소스들 생성할 때  다 이름추가해줘서 키(경로)말고 이름으로 화면에 띄우자
        char name_buff[50] = {};
        path res_name = material->GetKey();
        CopyStrToChar(res_name.stem().string(), name_buff, 50);

        ImGui::InputText("##MtrlUIName", name_buff, 50, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

        // GraphicsShader 이름
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

        // GraphicsShader 드랍 체크    
        if (ImGui::BeginDragDropTarget())
        {
            // 해당 노드에서 마우스 뗀 경우, 지정한 PayLoad 키값이 일치한 경우
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


        // Shader 에서 요구하는 ScalarParameter 를 UI 에 노출
        if (nullptr == shader)
            return 0;

        const vector<ScalarParam>& scalar_param = shader->GetScalarParam();

        for (size_t i = 0; i < scalar_param.size(); ++i)
        {
            switch (scalar_param[i].param)
            {
                // Shader Parameter 가 Int 타입인 경우
            case INT_0:
            case INT_1:
            case INT_2:
            case INT_3:
            {
                // 현재 머티리얼에 세팅된 값을 전달   
                int data = 0;
                material->GetScalarParam(scalar_param[i].param, &data);
                if (ParamUI::Param_Int(scalar_param[i].desc, &data))
                {
                    // UI 쪽에서 값이 변경되었으면, 실제 머티리얼 에도 적용
                    material->SetScalarParam(scalar_param[i].param, &data);
                }
            }
            break;
            case FLOAT_0:
            case FLOAT_1:
            case FLOAT_2:
            case FLOAT_3:
            {
                // 현재 머티리얼에 세팅된 값을 전달   
                float data = 0;
                material->GetScalarParam(scalar_param[i].param, &data);
                if (ParamUI::Param_Float(scalar_param[i].desc, &data))
                {
                    // UI 쪽에서 값이 변경되었으면, 실제 머티리얼 에도 적용
                    material->SetScalarParam(scalar_param[i].param, &data);
                }
            }
            break;
            case VEC2_0:
            case VEC2_1:
            case VEC2_2:
            case VEC2_3:
            {
                // 현재 머티리얼에 세팅된 값을 전달   
                vec2 data;
                material->GetScalarParam(scalar_param[i].param, &data);
                if (ParamUI::Param_Vec2(scalar_param[i].desc, &data))
                {
                    // UI 쪽에서 값이 변경되었으면, 실제 머티리얼 에도 적용
                    material->SetScalarParam(scalar_param[i].param, &data);
                }
            }
            break;
            case VEC4_0:
            case VEC4_1:
            case VEC4_2:
            case VEC4_3:
            {
                // 현재 머티리얼에 세팅된 값을 전달   
                vec4 data;
                material->GetScalarParam(scalar_param[i].param, &data);
                if (ParamUI::Param_Vec4(scalar_param[i].desc, &data))
                {
                    // UI 쪽에서 값이 변경되었으면, 실제 머티리얼 에도 적용
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

        // Shader 에서 요구하는 Texture Parameter 를 UI 에 노출
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