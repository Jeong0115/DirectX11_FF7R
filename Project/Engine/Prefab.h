#pragma once

#include "Resource.h"

namespace ff7r
{
	class GameObject;
	class Prefab : public Resource
	{
	public:
		Prefab();
		virtual ~Prefab();

		GameObject* Instantiate();
		GameObject* GetOriginalData() { return proto_object; }

		void RegisterProtoObject(GameObject* _proto_object);

		virtual int Save(const wstring& _relative_path) override;

	private:
		virtual int Load(const wstring& _file_path) override;
		GameObject* LoadGameObject(FILE* _file);

		int SaveGameObject(GameObject* _object, FILE* _file);
		virtual void UpdateData() override {}

	private:
		GameObject* proto_object;
	};
}
