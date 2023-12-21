#include "pch.h"

#include <Engine\Device.h>
#include <Engine\GameObject.h>
#include <Engine\LevelMgr.h>
#include <Engine\PathMgr.h>
#include <Engine\KeyMgr.h>
#include <Script\func.h>

#include "InspectorUI.h"
#include "ImGuiEventMgr.h"
#include "ContentUI.h"
#include "OutlinerUI.h"
#include "MenuUI.h"
#include "SceneUI.h"
#include "DeferredUI.h"
#include "SearchListUI.h"
#include "OptionListUI.h"
#include "UI.h"
#include "ParamUI.h"

#include "ImGuiMgr.h"
namespace ff7r
{
    ImGuiMgr::ImGuiMgr()
        : main_hwnd(nullptr)
        , contents_observer(nullptr)
    {

    }

    ImGuiMgr::~ImGuiMgr()
    {
        // ImGui Release
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        // UI 삭제
        Safe_Del_Map(map_ui);
    }


    void ImGuiMgr::Init(HWND _hwnd)
    {
        main_hwnd = _hwnd;

        // ImGui 초기화
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;
        //io.ConfigViewportsNoDefaultParent = true;
        //io.ConfigDockingAlwaysTabBar = true;
        //io.ConfigDockingTransparentPayload = true;
        //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
        //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
        }

        // Setup Platform/Renderer backends
        ImGui_ImplWin32_Init(main_hwnd);
        ImGui_ImplDX11_Init(DEVICE, CONTEXT);

        // Tool 용 UI 생성
        CreateUI();

        // Content 폴더 감시
        wstring content_path = PathMgr::GetInst()->GetContentPath();
        contents_observer = FindFirstChangeNotification(content_path.c_str(), true
            , FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME
            | FILE_ACTION_REMOVED | FILE_ACTION_ADDED);

        SetImGuiFocusCallback([]()->bool { return ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow); });
    }

    void ImGuiMgr::Progress()
    {
        Begin();
        Tick();
        FinalTick();       
        Render();
        ImGuiEventMgr::GetInst()->Tick();
        // Content 폴더 변경 감시
        ObserveContent();
    }

    void ImGuiMgr::Begin()
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ParamUI::Next_ID = 0;
    }

    void ImGuiMgr::Tick()
    {
        for (const auto& pair : map_ui)
        {
            pair.second->Tick();
        }
    }

    void ImGuiMgr::FinalTick()
    {
        // Demo UI
        ImGui::ShowDemoWindow();

        // InspectorUI
        for (const auto& pair : map_ui)
        {
            if (pair.second->IsActive())
            {
                pair.second->FinalTick();
            }
        }

        if (KEY_TAP(KEY::ENTER))
            ImGui::SetWindowFocus(nullptr);
    }

    void ImGuiMgr::Render()
    {
        // ImGui Rendering
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

    void ImGuiMgr::CreateUI()
    {
        UI* ui = nullptr;

        // InspectorUI
        ui = new InspectorUI;
        ui->SetActive(true);
        map_ui.insert(make_pair(ui->GetID(), ui));

        // ContentUI
        ui = new ContentUI;
        ui->SetActive(true);
        map_ui.insert(make_pair(ui->GetID(), ui));

        // Outliner
        ui = new OutlinerUI;
        ui->SetActive(true);
        map_ui.insert(make_pair(ui->GetID(), ui));

        // Menu
        ui = new MenuUI;
        ui->SetActive(true);
        map_ui.insert(make_pair(ui->GetID(), ui));

        // ListUI
        ui = new SearchListUI;
        ui->SetActive(false);
        map_ui.insert(make_pair(ui->GetID(), ui));

        ui = new SceneUI;
        ui->SetActive(true);
        map_ui.insert(make_pair(ui->GetID(), ui));

        ui = new DeferredUI;
        ui->SetActive(true);
        map_ui.insert(make_pair(ui->GetID(), ui));

        ui = new OptionListUI;
        ui->SetActive(false);
        map_ui.insert(make_pair(ui->GetID(), ui));

        for (const auto& pair : map_ui)
        {
            pair.second->Init();
        }
    }

    void ImGuiMgr::ObserveContent()
    {
        DWORD dwWaitStatus = WaitForSingleObject(contents_observer, 0);

        if (dwWaitStatus == WAIT_OBJECT_0)
        {
            // content 폴더에 변경점이 생겼다.
            ContentUI* UI = (ContentUI*)FindUI("##Content");
            UI->Reload();

            FindNextChangeNotification(contents_observer);
        }
    }


    UI* ImGuiMgr::FindUI(const string& _UIName)
    {
        map<string, UI*>::iterator iter = map_ui.find(_UIName);

        if (iter == map_ui.end())
            return nullptr;

        return iter->second;
    }
}