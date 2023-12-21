#pragma once

#ifndef SCRIP_FUNC
#define SCRIP_FUNC

namespace ff7r
{
    // ���߿� �ڵ� ����
    typedef bool (*ImGuiFocusCallback)();

    extern ImGuiFocusCallback ImGui_Focus_Callback; // ���� ���� ����

    void SetImGuiFocusCallback(ImGuiFocusCallback callback); // �Լ� ����
    bool IsImGuiFocused(); // �Լ� ����
}

#endif