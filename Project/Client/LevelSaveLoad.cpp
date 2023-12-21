#include "pch.h"

#include <Engine\PathMgr.h>
#include <Engine\LevelMgr.h>
#include <Engine\Level.h>
#include <Engine\Layer.h>
#include <Engine\GameObject.h>
#include <Engine\components.h>
#include <Engine\Script.h>

#include <Script\ScriptMgr.h>

#include "ImGuiMgr.h"
#include "InspectorUI.h"
#include "EditorObjMgr.h"

#include "LevelSaveLoad.h"
namespace ff7r
{
	int LevelSaveLoad::SaveLevel(const wstring& levelName, Level* _Level)
	{
		if (_Level->GetState() != LEVEL_STATE::STOP)
			return E_FAIL;

		wstring strPath = PathMgr::GetInst()->GetContentPath();
		strPath = strPath + L"Level\\" + levelName + L".lv";

		FILE* pFile = nullptr;

		_wfopen_s(&pFile, strPath.c_str(), L"wb");

		if (nullptr == pFile)
			return E_FAIL;

		// 레벨 이름 저장
		SaveWString(_Level->GetName(), pFile);


		// 레벨의 레이어들을 저장
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			Layer* pLayer = _Level->GetLayer(i);

			// 레이어 이름 저장
			SaveWString(pLayer->GetName(), pFile);

			// 레이어의 게임오브젝트들 저장
			const vector<GameObject*>& vecParent = pLayer->GetParentObject();

			// 오브젝트 개수 저장
			size_t objCount = vecParent.size();
			fwrite(&objCount, sizeof(size_t), 1, pFile);

			// 각 게임오브젝트
			for (size_t i = 0; i < objCount; ++i)
			{
				SaveGameObject(vecParent[i], pFile);
			}
		}

		fclose(pFile);

		return S_OK;
	}

	int LevelSaveLoad::SaveGameObject(GameObject* _Object, FILE* _File)
	{
		// 이름
		SaveWString(_Object->GetName(), _File);

		// 컴포넌트
		for (UINT i = 0; i <= (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (i == (UINT)COMPONENT_TYPE::END)
			{
				// 컴포넌트 타입 저장
				fwrite(&i, sizeof(UINT), 1, _File);
				break;
			}

			Component* Com = _Object->GetComponent((COMPONENT_TYPE)i);
			if (nullptr == Com)
				continue;

			// 컴포넌트 타입 저장
			fwrite(&i, sizeof(UINT), 1, _File);

			// 컴포넌트 정보 저장
			Com->SaveToLevelFile(_File);
		}

		// 스크립트	
		const vector<Script*>& vecScript = _Object->GetScripts();
		size_t ScriptCount = vecScript.size();
		fwrite(&ScriptCount, sizeof(size_t), 1, _File);

		for (size_t i = 0; i < vecScript.size(); ++i)
		{
			wstring ScriptName = ScriptMgr::GetScriptName(vecScript[i]);
			SaveWString(ScriptName, _File);
			vecScript[i]->SaveToLevelFile(_File);
		}


		// 자식 오브젝트
		const vector<GameObject*>& vecChild = _Object->GetChild();
		size_t ChildCount = vecChild.size();
		fwrite(&ChildCount, sizeof(size_t), 1, _File);

		for (size_t i = 0; i < ChildCount; ++i)
		{
			SaveGameObject(vecChild[i], _File);
		}

		return 0;
	}

	Level* LevelSaveLoad::LoadLevel(const wstring& _LevelPath)
	{
		EditorObjMgr::GetInst()->SetTransformTarget(nullptr);
		InspectorUI* Inspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
		Inspector->SetTargetObject(nullptr);

		wstring strPath = PathMgr::GetInst()->GetContentPath();
		strPath += _LevelPath;

		FILE* pFile = nullptr;

		_wfopen_s(&pFile, strPath.c_str(), L"rb");

		if (nullptr == pFile)
			return nullptr;

		Level* NewLevel = new Level;

		// 레벨 이름
		wstring strLevelName;
		LoadWString(strLevelName, pFile);
		NewLevel->SetName(strLevelName);


		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			Layer* pLayer = NewLevel->GetLayer(i);

			// 레이어 이름
			wstring LayerName;
			LoadWString(LayerName, pFile);
			pLayer->SetName(LayerName);

			// 게임 오브젝트 개수
			size_t objCount = 0;
			fread(&objCount, sizeof(size_t), 1, pFile);

			// 각 게임오브젝트
			for (size_t j = 0; j < objCount; ++j)
			{
				GameObject* pNewObj = LoadGameObject(pFile);
				NewLevel->AddGameObject(pNewObj, i, false);
			}
		}

		fclose(pFile);

		NewLevel->ChangeState(LEVEL_STATE::STOP);

		return NewLevel;
	}

	GameObject* LevelSaveLoad::LoadGameObject(FILE* _File)
	{
		GameObject* pObject = new GameObject;

		// 이름
		wstring Name;
		LoadWString(Name, _File);
		pObject->SetName(Name);

		// 컴포넌트
		while (true)
		{
			UINT ComponentType = 0;
			fread(&ComponentType, sizeof(UINT), 1, _File);

			// 컴포넌트 정보의 끝을 확인
			if ((UINT)COMPONENT_TYPE::END == ComponentType)
				break;

			Component* comp = nullptr;

			switch ((COMPONENT_TYPE)ComponentType)
			{
			case COMPONENT_TYPE::TRANSFORM:
				comp = new Transform;
				break;
			case COMPONENT_TYPE::COLLIDER2D:
				comp = new Collider2D;
				break;
			case COMPONENT_TYPE::COLLIDER3D:
				//Component = new CCollider2D;
				break;
			case COMPONENT_TYPE::ANIMATOR2D:
				comp = new Animator2D;
				break;
			case COMPONENT_TYPE::ANIMATOR3D:
				break;
			case COMPONENT_TYPE::LIGHT2D:
				comp = new Light2D;
				break;
			case COMPONENT_TYPE::LIGHT3D:
				comp = new Light3D;
				break;
			case COMPONENT_TYPE::CAMERA:
				comp = new Camera;
				break;
			case COMPONENT_TYPE::MESHRENDER:
				comp = new MeshRender;
				break;
			case COMPONENT_TYPE::PARTICLESYSTEM:
				comp = new ParticleSystem;
				break;
			case COMPONENT_TYPE::TILEMAP:
				comp = new TileMap;
				break;
			case COMPONENT_TYPE::LANDSCAPE:
				break;
			case COMPONENT_TYPE::DECAL:
				break;
			default: break;
			}

			comp->LoadFromLevelFile(_File);
			pObject->AddComponent(comp);
		}


		// 스크립트	
		size_t ScriptCount = 0;
		fread(&ScriptCount, sizeof(size_t), 1, _File);

		for (size_t i = 0; i < ScriptCount; ++i)
		{
			wstring ScriptName;
			LoadWString(ScriptName, _File);
			Script* script = ScriptMgr::GetScript(ScriptName);
			pObject->AddComponent(script);
			script->LoadFromLevelFile(_File);
		}

		// 자식 오브젝트		
		size_t ChildCount = 0;
		fread(&ChildCount, sizeof(size_t), 1, _File);

		for (size_t i = 0; i < ChildCount; ++i)
		{
			GameObject* ChildObject = LoadGameObject(_File);
			pObject->AddChild(ChildObject);
		}

		return pObject;
	}
}