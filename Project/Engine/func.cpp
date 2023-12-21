#include "pch.h"

#include "EventMgr.h"
#include "LevelMgr.h"
#include "Level.h"
#include "Layer.h"
#include "GameObject.h"
#include "RenderMgr.h"
#include "ResourceMgr.h"
#include "Transform.h"

#include "ptr.h"

namespace ff7r
{
	void SpawnGameObject(GameObject* _object, vec3 _world_pos, int _layer_idx)
	{
		_object->GetTransform()->SetRelativePos(_world_pos);

		Event event = {};

		event.type = EVENT_TYPE::CREATE_OBJECT;
		event.w_param = (DWORD_PTR)_object;
		event.l_param = _layer_idx;

		EventMgr::GetInst()->AddEvent(event);
	}

	void SpawnGameObject(GameObject* _object, vec3 _world_pos, const wstring& _layer_name)
	{
		_object->GetTransform()->SetRelativePos(_world_pos);

		Event evn = {};

		evn.type = EVENT_TYPE::CREATE_OBJECT;
		evn.w_param = (DWORD_PTR)_object;
		evn.l_param = LevelMgr::GetInst()->GetCurLevel()->FindLayerByName(_layer_name)->GetLayerIndex();

		EventMgr::GetInst()->AddEvent(evn);
	}

	void SpawnUIObject(GameObject* _object, vec3 _world_pos, const wstring& _layer_name)
	{
		_object->GetTransform()->SetRelativePos(_world_pos);

		Event evn = {};

		evn.type = EVENT_TYPE::CREATE_UI_OBJECT;
		evn.w_param = (DWORD_PTR)_object;
		evn.l_param = LevelMgr::GetInst()->GetCurLevel()->FindLayerByName(_layer_name)->GetLayerIndex();

		EventMgr::GetInst()->AddEvent(evn);
	}

	void DestroyObject(GameObject* _object)
	{
		if (_object->IsDead())
			return;

		Event evn = {};

		evn.type = EVENT_TYPE::DELETE_OBJECT;
		evn.w_param = (DWORD_PTR)_object;

		EventMgr::GetInst()->AddEvent(evn);
	}

	void DrawDebugRect(vec3 _world_pos, vec2 _world_scale, vec4 _color, vec3 _rotation, float _time)
	{
		DebugShapeInfo info = {};

		info.mat_world = XMMatrixIdentity();
		info.type = SHAPE_TYPE::RECT;
		info.max_time = _time;
		info.world_pos = _world_pos;
		info.world_scale = vec3(_world_scale.x, _world_scale.y, 1.f);
		info.world_rotation = _rotation;
		info.color = _color;

		RenderMgr::GetInst()->AddDebugShapeInfo(info);
	}

	void DrawDebugRect(const Matrix& _mat_world, vec4 _color, float _time)
	{
		DebugShapeInfo info = {};

		info.mat_world = _mat_world;
		info.type = SHAPE_TYPE::RECT;
		info.max_time = _time;
		info.color = _color;

		RenderMgr::GetInst()->AddDebugShapeInfo(info);
	}

	void DrawDebugCircle(vec3 _world_pos, float _radius, vec4 _color, vec3 _rotation, float _time)
	{
		DebugShapeInfo info = {};

		info.mat_world = XMMatrixIdentity();
		info.type = SHAPE_TYPE::CIRCLE;
		info.max_time = _time;
		info.world_pos = _world_pos;
		info.world_scale = vec3(_radius, _radius, 1.f);
		info.world_rotation = _rotation;
		info.color = _color;

		RenderMgr::GetInst()->AddDebugShapeInfo(info);
	}

	void DrawDebugCircle(const Matrix& _mat_world, vec4 _color, float _time)
	{
		DebugShapeInfo info = {};

		info.mat_world = _mat_world;
		info.type = SHAPE_TYPE::CIRCLE;
		info.max_time = _time;
		info.color = _color;

		RenderMgr::GetInst()->AddDebugShapeInfo(info);
	}


	bool IsValidObj(GameObject*& _target)
	{
		if (nullptr == _target)
			return false;

		if (_target->IsDead())
		{
			_target = nullptr;
			return false;
		}

		return true;
	}

	const char* ToString(RES_TYPE type)
	{
		return RES_TYPE_STR[(UINT)type];
	}

	const wchar_t* ToWString(RES_TYPE type)
	{
		return RES_TYPE_WSTR[(UINT)type];
	}


	const char* ToString(COMPONENT_TYPE type)
	{
		return COMPONENT_TYPE_STR[(UINT)type];
	}

	const wstring StrToWStr(const string& _str)
	{
		return wstring(_str.begin(), _str.end());
	}

	const string WStrToStr(const wstring& _wstr)
	{
		return string(_wstr.begin(), _wstr.end());
	}

	void CopyStrToChar(const string& _str, char* _buff, size_t _buff_size)
	{
		std::copy_n(_str.begin(), std::min(_str.length(), _buff_size - 1), _buff);
		_buff[std::min(_str.length(), _buff_size - 1)] = '\0';
	}

	void SaveWString(const wstring& _str, FILE* _file)
	{
		UINT iLen = (UINT)_str.length();
		fwrite(&iLen, sizeof(UINT), 1, _file);
		fwrite(_str.c_str(), sizeof(wchar_t), _str.length(), _file);
	}

	void LoadWString(wstring& _str, FILE* _file)
	{
		wchar_t szBuffer[256] = {};

		UINT iLen = 0;
		fread(&iLen, sizeof(UINT), 1, _file);
		fread(szBuffer, sizeof(wchar_t), iLen, _file);

		_str = szBuffer;
	}

	void SaveResRef(Ptr<Resource> _res, FILE* _file)
	{
		int i = 0;
		if (nullptr == _res)
		{
			fwrite(&i, sizeof(i), 1, _file);
		}
		else
		{
			i = 1;
			fwrite(&i, sizeof(i), 1, _file);
			SaveWString(_res->GetKey(), _file);
			SaveWString(_res->GetRelativePath(), _file);
		}
	}


	template<typename T>
	void LoadResRef(Ptr<T>& _Res, FILE* _File)
	{
		int i = 0;
		fread(&i, sizeof(i), 1, _File);

		if (i)
		{
			wstring strKey, strRelativePath;
			LoadWString(strKey, _File);
			LoadWString(strRelativePath, _File);

			_Res = ResourceMgr::GetInst()->Load<T>(strKey, strRelativePath);
		}
	}
	template void LoadResRef<Texture>(Ptr<Texture>& _res, FILE* _file);
	template void LoadResRef<Mesh>(Ptr<Mesh>& _res, FILE* _file);
	template void LoadResRef<Material>(Ptr<Material>& _res, FILE* _file);
	template void LoadResRef<GraphicsShader>(Ptr<GraphicsShader>& _res, FILE* _file);
	template void LoadResRef<Prefab>(Ptr<Prefab>& _res, FILE* _file);
	template void LoadResRef<Sound>(Ptr<Sound>& _res, FILE* _file);

	const wchar_t* ToWString(COMPONENT_TYPE type)
	{
		return COMPONENT_TYPE_WSTR[(UINT)type];
	}
}