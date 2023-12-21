#include "pch.h"

#include "Layer.h"
#include "Level.h"

#include "LevelMgr.h"

namespace ff7r
{
	LevelMgr::LevelMgr()
		: cur_level(nullptr)
	{
	}

	LevelMgr::~LevelMgr()
	{
		if (nullptr != cur_level)
		{
			delete cur_level;
		}
	}

	void LevelMgr::Init()
	{
		cur_level = new Level;
		cur_level->ChangeState(LEVEL_STATE::STOP);
	}

	void LevelMgr::Tick()
	{
		cur_level->Clear();

		if (LEVEL_STATE::PLAY == cur_level->GetState())
		{
			cur_level->Tick();
		}

		cur_level->FinalTick();
	}


	GameObject* LevelMgr::FindObjectByName(const wstring& _name)
	{
		return cur_level->FindObjectByName(_name);
	}

	void LevelMgr::FindObjectByName(const wstring& _name, vector<GameObject*>& _result)
	{
		cur_level->FindObjectByName(_name, _result);
	}

	void LevelMgr::ChangeLevel(Level* _next_level)
	{
		if (nullptr != cur_level)
		{
			delete cur_level;
			cur_level = nullptr;
		}

		cur_level = _next_level;
	}
}