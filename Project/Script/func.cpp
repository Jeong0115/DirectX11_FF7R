// ���� ���, scriptFunc.cpp

#include "pch.h"
#include "func.h"

namespace ff7r
{
    ImGuiFocusCallback ImGui_Focus_Callback = nullptr; // ���� ���� ����

    void SetImGuiFocusCallback(ImGuiFocusCallback callback) // �Լ� ����
    {
        ImGui_Focus_Callback = callback;
    }

    bool IsImGuiFocused() // �Լ� ����
    {
        if (ImGui_Focus_Callback)
        {
            return ImGui_Focus_Callback();
        }
        return false;
    }
}
