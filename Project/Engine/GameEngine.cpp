#include "pch.h"

#include "Device.h"
#include "PathMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "ResourceMgr.h"
#include "RenderMgr.h"
#include "LevelMgr.h"
#include "CollisionMgr.h"
#include "GLTFLoader.h"
#include "RenderMgr.h"
#include "EventMgr.h"
#include "FontMgr.h"

#include "GameEngine.h"

namespace ff7r
{
	Engine::Engine()
		: hWnd(nullptr)
	{
	}

	Engine::~Engine()
	{
	}

	int Engine::Init(HWND _hWnd, UINT _width, UINT _height)
	{
		hWnd = _hWnd;
		resolutioin = vec2((float)_width, (float)_height);

		RECT rt = { 0, 0, (int)_width, (int)_height };
		AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
		SetWindowPos(hWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);
		ShowWindow(hWnd, true);

		if (FAILED(Device::GetInst()->Init(hWnd, _width, _height)))
		{
			MessageBox(nullptr, L"Device 초기화 실패", L"에러", MB_OK);
			return E_FAIL;
		}

		PathMgr::GetInst()->Init();
		KeyMgr::GetInst()->Init();
		TimeMgr::GetInst()->Init();
		ResourceMgr::GetInst()->Init();
		RenderMgr::GetInst()->Init();
		FontMgr::GetInst()->Init();
		LevelMgr::GetInst()->Init();

		//GLTFLoader::GetInst()->LoadMesh("content\\PC0000_00.gltf");
		GLTFLoader::GetInst()->LoadMesh("content\\scene.gltf");

		return S_OK;
	}

	void Engine::Progress()
	{
		Tick();
		Render();

		EventMgr::GetInst()->Tick();
	}

	void Engine::Tick()
	{
		ResourceMgr::GetInst()->Tick();
		TimeMgr::GetInst()->Tick();
		KeyMgr::GetInst()->Tick();

		Sound::System->update();

		LevelMgr::GetInst()->Tick();
		CollisionMgr::GetInst()->Tick();
	}

	void Engine::Render()
	{
		RenderMgr::GetInst()->Render();
		TimeMgr::GetInst()->Render();
	}
}