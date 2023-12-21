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

		// 부모 UI
		if (nullptr == parent_ui)
		{
			// 모달리스
			if (!is_modal)
			{
				ImGui::Begin(strFullName.c_str(), &is_active);

				RenderUpdate();

				for (size_t i = 0; i < child_ui.size(); ++i)
				{
					// 자식UI 가 비활성화 상태면 건너뛴다.
					if (!child_ui[i]->IsActive())
						continue;

					child_ui[i]->FinalTick();

					// 자식 UI 간의 구분선
					if (i != child_ui.size() - 1)
						ImGui::Separator();
				}

				ImGui::End();
			}

			// 모달
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
						// 자식UI 가 비활성화 상태면 건너뛴다.
						if (!child_ui[i]->IsActive())
							continue;

						child_ui[i]->FinalTick();

						// 자식 UI 간의 구분선
						if (i != child_ui.size() - 1)
							ImGui::Separator();
					}

					ImGui::EndPopup();
				}
			}
		}

		// 자식 UI
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