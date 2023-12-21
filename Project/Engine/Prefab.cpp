#include "pch.h"

#include <Script/ScriptMgr.h>

#include "Component.h"
#include "GameObject.h"
#include "Script.h"
#include "PathMgr.h"


#include "Prefab.h"
namespace ff7r
{
	Prefab::Prefab()
		: Resource(RES_TYPE::PREFAB)
		, proto_object(nullptr)
	{
	}

	Prefab::~Prefab()
	{
		if (nullptr != proto_object)
			delete proto_object;
	}

	GameObject* Prefab::Instantiate()
	{
		return proto_object->Clone();
	}

	void Prefab::RegisterProtoObject(GameObject* _Proto)
	{
		// 원본 오브젝트는 레벨 소속이 아니여야 한다.
		//assert(-1 == _Proto->GetLayerIndex());

		proto_object = _Proto;
	}

	int Prefab::Load(const wstring& _file_path)
	{
		if (proto_object) return E_FAIL;

		FILE* file = nullptr;
		_wfopen_s(&file, _file_path.c_str(), L"rb");

		// Entity
		wstring name;
		LoadWString(name, file);
		SetName(name);

		// Res
		wstring key;
		LoadWString(key, file);

		wstring layer_idx;
		LoadWString(layer_idx, file);

		proto_object = LoadGameObject(file);
		proto_object->SetLayerIndex(std::stoi(layer_idx));
	}

	GameObject* Prefab::LoadGameObject(FILE* _file)
	{
		GameObject* object = new GameObject;

		// 이름
		wstring object_name;
		LoadWString(object_name, _file);
		object->SetName(object_name);

		// 컴포넌트
		while (true)
		{
			UINT comp_type = 0;
			fread(&comp_type, sizeof(UINT), 1, _file);

			// 컴포넌트 정보의 끝을 확인
			if ((UINT)COMPONENT_TYPE::END == comp_type)
				break;

			Component* component = nullptr;

			switch ((COMPONENT_TYPE)comp_type)
			{
			case COMPONENT_TYPE::TRANSFORM:
				component = new Transform;
				break;
			case COMPONENT_TYPE::COLLIDER2D:
				component = new Collider2D;
				break;
			case COMPONENT_TYPE::COLLIDER3D:
				//Component = new CCollider2D;
				break;
			case COMPONENT_TYPE::ANIMATOR2D:
				component = new Animator2D;
				break;
			case COMPONENT_TYPE::ANIMATOR3D:
				break;
			case COMPONENT_TYPE::LIGHT2D:
				component = new Light2D;
				break;
			case COMPONENT_TYPE::LIGHT3D:
				break;
			case COMPONENT_TYPE::CAMERA:
				component = new Camera;
				break;
			case COMPONENT_TYPE::MESHRENDER:
				component = new MeshRender;
				break;
			case COMPONENT_TYPE::PARTICLESYSTEM:
				component = new ParticleSystem;
				break;
			case COMPONENT_TYPE::TILEMAP:
				component = new TileMap;
				break;
			case COMPONENT_TYPE::LANDSCAPE:
				break;
			case COMPONENT_TYPE::DECAL:
				break;
			}

			component->LoadFromLevelFile(_file);
			object->AddComponent(component);
		}


		// 스크립트	
		size_t script_cnt = 0;
		fread(&script_cnt, sizeof(size_t), 1, _file);

		for (size_t i = 0; i < script_cnt; ++i)
		{
			wstring ScriptName;
			LoadWString(ScriptName, _file);
			Script* script = ScriptMgr::GetScript(ScriptName);
			object->AddComponent(script);
			script->LoadFromLevelFile(_file);
		}

		// 자식 오브젝트		
		size_t child_cnt = 0;
		fread(&child_cnt, sizeof(size_t), 1, _file);

		for (size_t i = 0; i < child_cnt; ++i)
		{
			GameObject* childe_object = LoadGameObject(_file);
			object->AddChild(childe_object);
		}

		return object;
	}

	int Prefab::Save(const wstring& _relative_path)
	{
		if (IsEngineRes())
			return E_FAIL;

		wstring file_path = PathMgr::GetInst()->GetContentPath();
		file_path += _relative_path;

		FILE* file = nullptr;
		_wfopen_s(&file, file_path.c_str(), L"wb");

		// Entity
		SaveWString(GetName(), file);

		// Res
		SaveWString(GetKey(), file);

		SaveWString(to_wstring(proto_object->GetLayerIndex()), file);
		SaveGameObject(proto_object, file);

		fclose(file);

		return S_OK;
	}

	int Prefab::SaveGameObject(GameObject* _object, FILE* _file)
	{
		SaveWString(_object->GetName(), _file);

		// 컴포넌트
		for (UINT i = 0; i <= (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (i == (UINT)COMPONENT_TYPE::END)
			{
				// 컴포넌트 타입 저장
				fwrite(&i, sizeof(UINT), 1, _file);
				break;
			}

			Component* component = _object->GetComponent((COMPONENT_TYPE)i);
			if (nullptr == component)
				continue;

			// 컴포넌트 타입 저장
			fwrite(&i, sizeof(UINT), 1, _file);

			// 컴포넌트 정보 저장
			component->SaveToLevelFile(_file);
		}

		// 스크립트	
		const vector<Script*>& scripts = _object->GetScripts();
		size_t script_cnt = scripts.size();
		fwrite(&script_cnt, sizeof(size_t), 1, _file);

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			wstring ScriptName = ScriptMgr::GetScriptName(scripts[i]);
			SaveWString(ScriptName, _file);
			scripts[i]->SaveToLevelFile(_file);
		}


		// 자식 오브젝트
		const vector<GameObject*>& child_object = _object->GetChild();
		size_t child_count = child_object.size();
		fwrite(&child_count, sizeof(size_t), 1, _file);

		for (size_t i = 0; i < child_count; ++i)
		{
			SaveGameObject(child_object[i], _file);
		}

		return S_OK;
	}
}
