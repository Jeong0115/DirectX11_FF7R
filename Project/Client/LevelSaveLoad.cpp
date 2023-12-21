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

		// ���� �̸� ����
		SaveWString(_Level->GetName(), pFile);


		// ������ ���̾���� ����
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			Layer* pLayer = _Level->GetLayer(i);

			// ���̾� �̸� ����
			SaveWString(pLayer->GetName(), pFile);

			// ���̾��� ���ӿ�����Ʈ�� ����
			const vector<GameObject*>& vecParent = pLayer->GetParentObject();

			// ������Ʈ ���� ����
			size_t objCount = vecParent.size();
			fwrite(&objCount, sizeof(size_t), 1, pFile);

			// �� ���ӿ�����Ʈ
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
		// �̸�
		SaveWString(_Object->GetName(), _File);

		// ������Ʈ
		for (UINT i = 0; i <= (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (i == (UINT)COMPONENT_TYPE::END)
			{
				// ������Ʈ Ÿ�� ����
				fwrite(&i, sizeof(UINT), 1, _File);
				break;
			}

			Component* Com = _Object->GetComponent((COMPONENT_TYPE)i);
			if (nullptr == Com)
				continue;

			// ������Ʈ Ÿ�� ����
			fwrite(&i, sizeof(UINT), 1, _File);

			// ������Ʈ ���� ����
			Com->SaveToLevelFile(_File);
		}

		// ��ũ��Ʈ	
		const vector<Script*>& vecScript = _Object->GetScripts();
		size_t ScriptCount = vecScript.size();
		fwrite(&ScriptCount, sizeof(size_t), 1, _File);

		for (size_t i = 0; i < vecScript.size(); ++i)
		{
			wstring ScriptName = ScriptMgr::GetScriptName(vecScript[i]);
			SaveWString(ScriptName, _File);
			vecScript[i]->SaveToLevelFile(_File);
		}


		// �ڽ� ������Ʈ
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

		// ���� �̸�
		wstring strLevelName;
		LoadWString(strLevelName, pFile);
		NewLevel->SetName(strLevelName);


		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			Layer* pLayer = NewLevel->GetLayer(i);

			// ���̾� �̸�
			wstring LayerName;
			LoadWString(LayerName, pFile);
			pLayer->SetName(LayerName);

			// ���� ������Ʈ ����
			size_t objCount = 0;
			fread(&objCount, sizeof(size_t), 1, pFile);

			// �� ���ӿ�����Ʈ
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

		// �̸�
		wstring Name;
		LoadWString(Name, _File);
		pObject->SetName(Name);

		// ������Ʈ
		while (true)
		{
			UINT ComponentType = 0;
			fread(&ComponentType, sizeof(UINT), 1, _File);

			// ������Ʈ ������ ���� Ȯ��
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


		// ��ũ��Ʈ	
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

		// �ڽ� ������Ʈ		
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