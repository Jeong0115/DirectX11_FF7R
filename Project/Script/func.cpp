// 예를 들어, scriptFunc.cpp

#include "pch.h"
#include "func.h"

namespace ff7r
{
    ImGuiFocusCallback ImGui_Focus_Callback = nullptr; // 전역 변수 정의

    void SetImGuiFocusCallback(ImGuiFocusCallback callback) // 함수 정의
    {
        ImGui_Focus_Callback = callback;
    }

    bool IsImGuiFocused() // 함수 정의
    {
        if (ImGui_Focus_Callback)
        {
            return ImGui_Focus_Callback();
        }
        return false;
    }
}
