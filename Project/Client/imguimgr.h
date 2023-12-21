#pragma once


#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_dx11.h"
#include "ImGui\imgui_impl_win32.h"


// ========
// Delegate
// ========
namespace ff7r
{
    class UI;

    typedef void (UI::* UI_DELEGATE)(void);
    typedef void (UI::* UI_DELEGATE_1)(DWORD_PTR);
    typedef void (UI::* UI_DELEGATE_2)(DWORD_PTR, DWORD_PTR);

    class ImGuiMgr : public Singleton<ImGuiMgr>
    {
    public:
        void    Init(HWND _hWnd);
        void    Progress();

        UI*     FindUI(const string& _ui_name);
        HWND    GetMainHwnd() { return main_hwnd; }

    private:
        SINGLE(ImGuiMgr);

        void CreateUI();
        void ObserveContent();

        void Begin();
        void Tick();
        void FinalTick();
        void Render();

    private:
        HWND                main_hwnd;
        map<string, UI*>    map_ui;
        HANDLE              contents_observer;
    };

}