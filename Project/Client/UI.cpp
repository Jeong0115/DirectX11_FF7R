#include "pch.h"
#include "UI.h"

namespace ff7r
{
	UI::UI(const string& _ID)
		: id(_ID)
		, parent_ui(nullptr)
		, is_modal(false)
		, is_active(false)
	{

	}

	UI::~UI()
	{
		Safe_Del_Vec(child_ui);
	}

	void UI::FinalTick()
	{
		if (!is_active)
			return;

		string strFullName = name + id;

		// �θ� UI
		if (nullptr == parent_ui)
		{
			// ��޸���
			if (!is_modal)
			{
				ImGui::Begin(strFullName.c_str(), &is_active);

				RenderUpdate();

				for (size_t i = 0; i < child_ui.size(); ++i)
				{
					// �ڽ�UI �� ��Ȱ��ȭ ���¸� �ǳʶڴ�.
					if (!child_ui[i]->IsActive())
						continue;

					child_ui[i]->FinalTick();

					// �ڽ� UI ���� ���м�
					if (i != child_ui.size() - 1)
						ImGui::Separator();
				}

				ImGui::End();
			}

			// ���
			else
			{
				ImGui::SetNextWindowPos(pos);
				ImGui::SetNextWindowSize(size);

				ImGui::OpenPopup(strFullName.c_str());
				if (ImGui::BeginPopupModal(strFullName.c_str(), &is_active))
				{
					RenderUpdate();

					for (size_t i = 0; i < child_ui.size(); ++i)
					{
						// �ڽ�UI �� ��Ȱ��ȭ ���¸� �ǳʶڴ�.
						if (!child_ui[i]->IsActive())
							continue;

						child_ui[i]->FinalTick();

						// �ڽ� UI ���� ���м�
						if (i != child_ui.size() - 1)
							ImGui::Separator();
					}

					ImGui::EndPopup();
				}
			}
		}

		// �ڽ� UI
		else
		{
			ImGui::BeginChild(strFullName.c_str(), size);

			RenderUpdate();

			for (size_t i = 0; i < child_ui.size(); ++i)
			{
				child_ui[i]->FinalTick();

				if (i != child_ui.size() - 1)
					ImGui::Separator();
			}

			ImGui::EndChild();
		}
	}
}