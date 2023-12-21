#include "pch.h"
#include "TransformUI.h"

#include <Engine\GameObject.h>
#include <Engine\Transform.h>

namespace ff7r
{
	TransformUI::TransformUI()
		: ComponentUI("##Transform", COMPONENT_TYPE::TRANSFORM)
	{
		SetName("Transform");
	}

	TransformUI::~TransformUI()
	{
	}

	int TransformUI::RenderUpdate()
	{
		if (FALSE == ComponentUI::RenderUpdate())
			return FALSE;

		vec3 vPos = GetTarget()->GetTransform()->GetRelativePos();
		vec3 vScale = GetTarget()->GetTransform()->GetRelativeScale();
		vec3 vRotation = GetTarget()->GetTransform()->GetRelativeRot();
		vRotation = (vRotation / XM_PI) * 180.f;

		const char* dir[] = { "X", "Y", "Z" };

		ImGui::Text("Position");
		for (int i = 0; i < 3; i++)
		{
			ImGui::SameLine();
			ImGui::PushID(i);
			ImGui::Text(dir[i]);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(80.0f);
			ImGui::DragFloat("##Relative Position", &vPos[i]);
			ImGui::PopID();
		}

		ImGui::Text("Scale   ");
		for (int i = 0; i < 3; i++)
		{
			ImGui::SameLine();
			ImGui::PushID(i);
			ImGui::Text(dir[i]);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(80.0f);
			ImGui::DragFloat("##Relative Scale", &vScale[i]);
			ImGui::PopID();
		}

		ImGui::Text("Rotation");
		for (int i = 0; i < 3; i++)
		{
			ImGui::SameLine();
			ImGui::PushID(i);
			ImGui::Text(dir[i]);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(80.0f);
			ImGui::DragFloat("##Relative Rotation", &vRotation[i]);
			ImGui::PopID();
		}

		GetTarget()->GetTransform()->SetRelativePos(vPos);
		GetTarget()->GetTransform()->SetRelativeScale(vScale);

		vRotation = (vRotation / 180.f) * XM_PI;
		GetTarget()->GetTransform()->SetRelativeRot(vRotation);

		return TRUE;
	}
}