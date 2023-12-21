#include "pch.h"
#include "SearchListUI.h"

#include <Engine/KeyMgr.h>
#include <Engine/ResourceMgr.h>
#include <Script/ScriptMgr.h>
namespace ff7r
{
	auto ToLower = [](const std::string& str) -> std::string {
		std::string lowerStr = str;
		std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
		return lowerStr;
		};


	SearchListUI::SearchListUI()
		: UI("##SearchListUI")
		, select_inst(nullptr)
		, select_delegate(nullptr)
		, is_first_frame(false)
		, search_buff{}
	{
	}

	SearchListUI::~SearchListUI()
	{
	}

	void SearchListUI::FinalTick()
	{
		UI::FinalTick();

		// Esc 눌리면 비활성화
		if (KEY_TAP(KEY::ESC))
		{
			Close();
		}
	}

	int SearchListUI::RenderUpdate()
	{
		if (is_first_frame)
		{
			ImGui::SetKeyboardFocusHere();
			is_first_frame = false;
		}

		ImGui::InputText("Search", search_buff, IM_ARRAYSIZE(search_buff), ImGuiInputTextFlags_EnterReturnsTrue);
		string lowerSearchBuffer = ToLower(search_buff);
		string firstResult;

		ImVec2 ListUIContentSize = ImGui::GetContentRegionAvail();
		if (ImGui::BeginListBox("##Search", ListUIContentSize))
		{
			bool bFirstResult = true;
			for (int i = 0; i < string_data.size(); i++)
			{
				//string item_name = m_vecStrData[i].get
				string lowerItem = ToLower(string_data[i]);
				if (lowerItem.find(lowerSearchBuffer) != string::npos)
				{
					if (bFirstResult)
					{
						firstResult = string_data[i];
						bFirstResult = false;
					}
					if (ImGui::Selectable(path(string_data[i]).stem().string().c_str()))
					{
						Close();
						(select_inst->*select_delegate)((DWORD_PTR)string_data[i].c_str());
					}
				}
			}
			ImGui::EndListBox();
		}

		if (ImGui::IsKeyDown(ImGuiKey_Enter))
		{
			Close();
			(select_inst->*select_delegate)((DWORD_PTR)firstResult.c_str());
		}
		else if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
		{
			Close();
		}


		return 0;
	}

	void SearchListUI::Reset(SearchListType _type, ImVec2 _size)
	{
		Clear();
		//SetSize(_vSize.x, _vSize.y);
		SetActive(true);
		is_first_frame = true;

		switch (_type)
		{
		case SearchListType::COMPONENTS:
			AddComponents();
			SetName("Components");
			break;

		case SearchListType::MATERIALS:
			AddMaterials();
			SetName("Materials");
			break;

		case SearchListType::MESHES:
			AddMeshes();
			SetName("Meshes");
			break;

		case SearchListType::SOUNDS:
			AddSounds();
			SetName("Sounds");
			break;

		case SearchListType::TEXTURES:
			AddTextures();
			SetName("Textures");
			break;

		default: break;
		}


		//// 위치 조정
		//HWND hWnd = ImGuiMgr::GetInst()->GetMainHwnd();
		//RECT rt = {};
		////GetClientRect(hWnd, &rt);
		//GetWindowRect(hWnd, &rt);
		//
		//float CenterX = (float)((rt.left + rt.right) / 2);
		//float CenterY = (float)((rt.bottom + rt.top) / 2);
		//
		//SetPopupPos(ImVec2(CenterX - _vSize.x / 2.f, CenterY - _vSize.y / 2.f));
	}

	void SearchListUI::Close()
	{
		SetActive(false);
		memset(search_buff, 0, IM_ARRAYSIZE(search_buff));
	}

	void SearchListUI::AddComponents()
	{
		vector<wstring> vecScripts;
		ScriptMgr::GetScriptInfo(vecScripts);

		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			AddItem(ToString((COMPONENT_TYPE)i));
		}
		for (const auto& name : vecScripts)
		{
			AddItem(string(name.begin(), name.end()));
		}
	}

	void SearchListUI::AddMaterials()
	{
		const map<wstring, Ptr<Resource>>& mapMtrl = ResourceMgr::GetInst()->GetResources(RES_TYPE::MATERIAL);

		for (const auto& pair : mapMtrl)
		{
			AddItem(string(pair.first.begin(), pair.first.end()));
		}
	}

	void SearchListUI::AddTextures()
	{
		const map<wstring, Ptr<Resource>>& mapTex = ResourceMgr::GetInst()->GetResources(RES_TYPE::TEXTURE);

		for (const auto& pair : mapTex)
		{
			AddItem(string(pair.first.begin(), pair.first.end()));
		}
	}

	void SearchListUI::AddMeshes()
	{
		const map<wstring, Ptr<Resource>>& mapMesh = ResourceMgr::GetInst()->GetResources(RES_TYPE::MESH);

		for (const auto& pair : mapMesh)
		{
			AddItem(string(pair.first.begin(), pair.first.end()));
		}
	}

	void SearchListUI::AddSounds()
	{
	}
}