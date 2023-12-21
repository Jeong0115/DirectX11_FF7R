#include "pch.h"
#include "InspectorUI.h"

#include <Engine\GameObject.h>
#include <Engine\Transform.h>
#include <Engine\LevelMgr.h>
#include <Engine\KeyMgr.h>

#include "TransformUI.h"
#include "MeshRenderUI.h"
#include "Collider2DUI.h"
#include "CameraUI.h"
#include "Animator2DUI.h"
#include "TileMapUI.h"
#include "Light2DUI.h"

#include "MeshDataUI.h"
#include "TextureUI.h"
#include "MeshUI.h"
#include "SoundUI.h"
#include "PrefabUI.h"
#include "GraphicsShaderUI.h"
#include "ComputeShaderUI.h"
#include "ComponentUI.h"
#include "MaterialUI.h"

#include "MeshRenderUI.h"
#include "SkyBoxUI.h"
#include "ScriptUI.h"
#include "LevelUI.h"
#include "ResUI.h"

#include "AddComponentUI.h"
#include "SearchListUI.h"
#include <Script/ScriptMgr.h>

namespace ff7r
{
	InspectorUI::InspectorUI()
		: UI("##Inspector")
		, target_obj(nullptr)
		, components_ui{}
		, resources_ui{}
	{
		SetName("Inspector");

		components_ui[(UINT)COMPONENT_TYPE::TRANSFORM] = new TransformUI;
		components_ui[(UINT)COMPONENT_TYPE::TRANSFORM]->SetSize(0.f, 150.f);
		AddChildUI(components_ui[(UINT)COMPONENT_TYPE::TRANSFORM]);

		components_ui[(UINT)COMPONENT_TYPE::MESHRENDER] = new MeshRenderUI;
		components_ui[(UINT)COMPONENT_TYPE::MESHRENDER]->SetSize(0.f, 150.f);
		AddChildUI(components_ui[(UINT)COMPONENT_TYPE::MESHRENDER]);

		components_ui[(UINT)COMPONENT_TYPE::CAMERA] = new CameraUI;
		components_ui[(UINT)COMPONENT_TYPE::CAMERA]->SetSize(0.f, 150.f);
		AddChildUI(components_ui[(UINT)COMPONENT_TYPE::CAMERA]);

		components_ui[(UINT)COMPONENT_TYPE::COLLIDER2D] = new Collider2DUI;
		components_ui[(UINT)COMPONENT_TYPE::COLLIDER2D]->SetSize(0.f, 150.f);
		AddChildUI(components_ui[(UINT)COMPONENT_TYPE::COLLIDER2D]);

		components_ui[(UINT)COMPONENT_TYPE::ANIMATOR2D] = new Animator2DUI;
		components_ui[(UINT)COMPONENT_TYPE::ANIMATOR2D]->SetSize(0.f, 150.f);
		AddChildUI(components_ui[(UINT)COMPONENT_TYPE::ANIMATOR2D]);

		components_ui[(UINT)COMPONENT_TYPE::LIGHT2D] = new Light2DUI;
		components_ui[(UINT)COMPONENT_TYPE::LIGHT2D]->SetSize(0.f, 150.f);
		AddChildUI(components_ui[(UINT)COMPONENT_TYPE::LIGHT2D]);

		components_ui[(UINT)COMPONENT_TYPE::TILEMAP] = new TileMapUI;
		components_ui[(UINT)COMPONENT_TYPE::TILEMAP]->SetSize(0.f, 150.f);
		AddChildUI(components_ui[(UINT)COMPONENT_TYPE::TILEMAP]);

		components_ui[(UINT)COMPONENT_TYPE::SKYBOX] = new SkyBoxUI;
		components_ui[(UINT)COMPONENT_TYPE::SKYBOX]->SetSize(0.f, 150.f);
		AddChildUI(components_ui[(UINT)COMPONENT_TYPE::SKYBOX]);

		// ResUI
		resources_ui[(UINT)RES_TYPE::LEVEL] = new LevelUI;
		resources_ui[(UINT)RES_TYPE::LEVEL]->SetSize(0.f, 0.f);
		AddChildUI(resources_ui[(UINT)RES_TYPE::LEVEL]);

		resources_ui[(UINT)RES_TYPE::MESHDATA] = new MeshDataUI;
		resources_ui[(UINT)RES_TYPE::MESHDATA]->SetSize(0.f, 0.f);
		AddChildUI(resources_ui[(UINT)RES_TYPE::MESHDATA]);

		resources_ui[(UINT)RES_TYPE::MESH] = new MeshUI;
		resources_ui[(UINT)RES_TYPE::MESH]->SetSize(0.f, 0.f);
		AddChildUI(resources_ui[(UINT)RES_TYPE::MESH]);

		resources_ui[(UINT)RES_TYPE::TEXTURE] = new TextureUI;
		resources_ui[(UINT)RES_TYPE::TEXTURE]->SetSize(0.f, 0.f);
		AddChildUI(resources_ui[(UINT)RES_TYPE::TEXTURE]);

		resources_ui[(UINT)RES_TYPE::GRAPHICS_SHADER] = new GraphicsShaderUI;
		resources_ui[(UINT)RES_TYPE::GRAPHICS_SHADER]->SetSize(0.f, 0.f);
		AddChildUI(resources_ui[(UINT)RES_TYPE::GRAPHICS_SHADER]);

		resources_ui[(UINT)RES_TYPE::COMPUTE_SHADER] = new ComputeShaderUI;
		resources_ui[(UINT)RES_TYPE::COMPUTE_SHADER]->SetSize(0.f, 0.f);
		AddChildUI(resources_ui[(UINT)RES_TYPE::COMPUTE_SHADER]);

		resources_ui[(UINT)RES_TYPE::PREFAB] = new PrefabUI;
		resources_ui[(UINT)RES_TYPE::PREFAB]->SetSize(0.f, 0.f);
		AddChildUI(resources_ui[(UINT)RES_TYPE::PREFAB]);

		resources_ui[(UINT)RES_TYPE::MATERIAL] = new MaterialUI;
		resources_ui[(UINT)RES_TYPE::MATERIAL]->SetSize(0.f, 0.f);
		AddChildUI(resources_ui[(UINT)RES_TYPE::MATERIAL]);

		resources_ui[(UINT)RES_TYPE::SOUND] = new SoundUI;
		resources_ui[(UINT)RES_TYPE::SOUND]->SetSize(0.f, 0.f);
		AddChildUI(resources_ui[(UINT)RES_TYPE::SOUND]);

		//mCompUI = new AddComponentUI;
	}

	InspectorUI::~InspectorUI()
	{

	}

	void InspectorUI::Init()
	{
		SetTargetObject(nullptr);
		//mCompUI->init();
	}

	void InspectorUI::Tick()
	{

	}

	void InspectorUI::FinalTick()
	{
		if (!is_active)
			return;

		string strFullName = name + id;

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

		AddComponentToTargetObject();

		ImGui::End();
	}

	int InspectorUI::RenderUpdate()
	{

		return TRUE;
	}

	void InspectorUI::SetTargetObject(GameObject* _Target)
	{
		ClearTargetResource();

		// 타겟오브젝트 정보 노출
		target_obj = _Target;

		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (nullptr == components_ui[i])
				continue;

			components_ui[i]->SetTarget(target_obj);
		}

		// 타겟 오브젝트가 nullptr 이면
		// 스크립트UI 들을 전부 비활성화 시킨다.
		if (nullptr == target_obj)
		{
			for (size_t i = 0; i < scripts_ui.size(); ++i)
			{
				scripts_ui[i]->SetActive(false);
			}
			return;
		}

		// 오브젝트의 스크립트 목록을 받아온다.
		const vector<Script*>& vecScript = target_obj->GetScripts();

		// 스크립트UI 가 스크립트 수 보다 적으면 그만큼 추가해준다.
		if (scripts_ui.size() < vecScript.size())
		{
			UINT iDiffer = vecScript.size() - scripts_ui.size();
			for (UINT i = 0; i < iDiffer; ++i)
			{
				ScriptUI* UI = new ScriptUI;

				scripts_ui.push_back(UI);
				AddChildUI(UI);
				UI->SetActive(true);
			}
		}

		// ScriptUI 반복문 돌면서 오브젝트의 스크립트수 만큼만 활성화 시킨다.
		for (size_t i = 0; i < scripts_ui.size(); ++i)
		{
			if (vecScript.size() <= i)
			{
				scripts_ui[i]->SetActive(false);
				continue;
			}

			// 스크립트를 스크립트UI 에게 알려준다.
			scripts_ui[i]->SetTarget(target_obj);
			scripts_ui[i]->SetScript(vecScript[i]);
			scripts_ui[i]->SetActive(true);
			scripts_ui[i]->SetSize(0.0f, 150.f);
		}
	}

	void InspectorUI::SetTargetResource(Ptr<Resource> _Res)
	{
		ClearTargetObject();

		for (UINT i = 0; i < UINT(RES_TYPE::END); ++i)
		{
			resources_ui[i]->SetActive(false);
		}

		target_res = _Res;

		if (nullptr == target_res)
			return;

		RES_TYPE type = _Res->GetType();

		resources_ui[(UINT)type]->SetActive(true);
		resources_ui[(UINT)type]->SetTargetRes(_Res);
	}

	void InspectorUI::AddComponent(DWORD_PTR _Key)
	{
		if (!target_obj) return;

		string strKey = (char*)_Key;
		wstring wstrKey = wstring(strKey.begin(), strKey.end());

		Script* script = ScriptMgr::GetScript(wstrKey);
		if (script)
		{
			target_obj->AddComponent(script);
		}
		else
		{
			COMPONENT_TYPE _type = COMPONENT_TYPE::END;

			for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
			{
				if (strKey == ToString((COMPONENT_TYPE)i))
				{
					_type = (COMPONENT_TYPE)i;
					break;
				}
			}

			switch (_type)
			{
			case COMPONENT_TYPE::TRANSFORM:
				target_obj->AddComponent(new Transform);
				break;
			case COMPONENT_TYPE::COLLIDER2D:
				target_obj->AddComponent(new Collider2D);
				break;
			case COMPONENT_TYPE::COLLIDER3D:
				//pSelectedObject->AddComponent(new CCollider3D);
				break;
			case COMPONENT_TYPE::ANIMATOR2D:
				target_obj->AddComponent(new Animator2D);
				break;
			case COMPONENT_TYPE::ANIMATOR3D:
				//pSelectedObject->AddComponent(new CAnimator3D);
				break;
			case COMPONENT_TYPE::LIGHT2D:
				target_obj->AddComponent(new Light2D);
				break;
			case COMPONENT_TYPE::LIGHT3D:
				//pSelectedObject->AddComponent(new CLight3D);
				break;
			case COMPONENT_TYPE::CAMERA:
				target_obj->AddComponent(new Camera);
				break;
			case COMPONENT_TYPE::MESHRENDER:
				target_obj->AddComponent(new MeshRender);
				break;
			case COMPONENT_TYPE::PARTICLESYSTEM:
				target_obj->AddComponent(new ParticleSystem);
				break;
			case COMPONENT_TYPE::TILEMAP:
				target_obj->AddComponent(new TileMap);
				break;
			case COMPONENT_TYPE::LANDSCAPE:
				//pSelectedObject->AddComponent(new CLandScape);
				break;
			case COMPONENT_TYPE::DECAL:
				//pSelectedObject->AddComponent(new CDecal);
				break;
			}
		}

		SetTargetObject(target_obj);
	}

	void InspectorUI::AddComponentToTargetObject()
	{
		if (!target_obj) return;

		ImGui::BeginChild("Add");

		//ImGui::Button("Add Component");

		if (ImGui::Button(" Add Component ", ImVec2(120, 20)))
		{
			SearchListUI* pListUI = (SearchListUI*)ImGuiMgr::GetInst()->FindUI("##SearchListUI");
			pListUI->Reset(SearchListUI::SearchListType::COMPONENTS, ImVec2(300.f, 500.f));
			pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&InspectorUI::AddComponent);
		}


		ImGui::EndChild();
	}

	void InspectorUI::ClearTargetObject()
	{
		// 타겟오브젝트 정보 노출
		target_obj = nullptr;

		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (nullptr == components_ui[i])
				continue;

			components_ui[i]->SetTarget(nullptr);
		}
	}

	void InspectorUI::ClearTargetResource()
	{
		target_res = nullptr;

		for (UINT i = 0; i < UINT(RES_TYPE::END); ++i)
		{
			if (nullptr != resources_ui[i])
			{
				resources_ui[i]->SetTargetRes(nullptr);
				resources_ui[i]->SetActive(false);
			}
		}
	}
}