#include "pch.h"
#include "MeshUI.h"

#include <Engine\Mesh.h>
namespace ff7r
{
    MeshUI::MeshUI()
        : ResourceUI(RES_TYPE::MESH)
    {
        SetName("Mesh");
    }

    MeshUI::~MeshUI()
    {
    }

    int MeshUI::RenderUpdate()
    {
        ResourceUI::RenderUpdate();

        ImGui::Text("Mesh   ");
        ImGui::SameLine();

        Ptr<Mesh> pMesh = (Mesh*)GetTargetRes().Get();
        string strKey = string(pMesh->GetKey().begin(), pMesh->GetKey().end());
        ImGui::InputText("##MeshUIName", (char*)strKey.c_str(), strKey.length(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

        return 0;
    }
}