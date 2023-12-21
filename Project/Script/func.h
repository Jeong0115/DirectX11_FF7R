#pragma once

#ifndef SCRIP_FUNC
#define SCRIP_FUNC

namespace ff7r
{
    // 나중에 코드 수정
    typedef bool (*ImGuiFocusCallback)();

    extern ImGuiFocusCallback ImGui_Focus_Callback; // 전역 변수 선언

    void SetImGuiFocusCallback(ImGuiFocusCallback callback); // 함수 선언
    bool IsImGuiFocused(); // 함수 선언
}

#endif