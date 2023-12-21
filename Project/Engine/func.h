#pragma once

namespace ff7r
{
	// 오브젝트 생성
	class GameObject;
	void SpawnGameObject(GameObject* _object, vec3 _world_pos, int _layer_idx);
	void SpawnGameObject(GameObject* _object, vec3 _world_pos, const wstring& _layer_name);
	void SpawnUIObject(GameObject* _object, vec3 _world_pos, const wstring& _layer_name = L"UI");

	// 오브젝트 삭제
	void DestroyObject(GameObject* _object);

	// DrawDebugShape
	void DrawDebugRect(vec3 _world_pos, vec2 _world_scale, vec4 _color, vec3 _rotation, float _time = 0.f);
	void DrawDebugRect(const Matrix& _mat_world, vec4 _color, float _time = 0.f);

	void DrawDebugCircle(vec3 _world_pos, float _radius, vec4 _color, vec3 _rotation, float _time = 0.f);
	void DrawDebugCircle(const Matrix& _mat_world, vec4 _color, float _time = 0.f);

	// void DrawDebugSphere();
	// void DrawDebugCube();

	// GameObject 유효성 체크
	bool IsValidObj(GameObject*& _target);


	const char* ToString(RES_TYPE);
	const wchar_t* ToWSTring(RES_TYPE);

	const char* ToString(COMPONENT_TYPE);
	const wchar_t* ToWSTring(COMPONENT_TYPE);

	const wstring StrToWStr(const string& _str);
	const string WStrToStr(const wstring& _wstr);

	void CopyStrToChar(const string& _str, char* _buff, size_t _buff_size);

	// Save / Load
	void SaveWString(const wstring& _str, FILE* _file);
	void LoadWString(wstring& _str, FILE* _file);

	class Resource;
	template<typename T>
	class Ptr;
	void SaveResRef(Ptr<Resource> _res, FILE* _file);

	//우선은 임시로 해결했는데 나중에 질문하자...
	template<typename T>
	void LoadResRef(Ptr<T>& _res, FILE* _file);


	template<typename T, UINT Size>
	void Safe_Del_Array(T* (&arr)[Size])
	{
		for (UINT i = 0; i < Size; ++i)
		{
			if (nullptr != arr[i])
				delete arr[i];
		}
	}

	template<typename T>
	void Safe_Del_Point(T* point)
	{
		if (point)
		{
			delete point;
			point = nullptr;
		}
	}

	template<typename T>
	void Safe_Del_Vec(vector<T*>& _vec)
	{
		for (size_t i = 0; i < _vec.size(); ++i)
		{
			if (nullptr != _vec[i])
			{
				delete _vec[i];
			}
		}
		_vec.clear();
	}

	template<typename T1, typename T2>
	void Safe_Del_Map(map<T1, T2>& _map)
	{
		for (const auto& pair : _map)
		{
			if (nullptr != pair.second)
				delete pair.second;
		}

		_map.clear();
	}
}


