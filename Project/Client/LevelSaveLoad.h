#pragma once


namespace ff7r
{
	class Level;
	class LevelSaveLoad
	{
	public:
		static int SaveLevel(const wstring& levelName, Level* _Level);
		static int SaveGameObject(GameObject* _Object, FILE* _File);

		static Level* LoadLevel(const wstring& _LevelPath);
		static GameObject* LoadGameObject(FILE* _File);
	};
}
