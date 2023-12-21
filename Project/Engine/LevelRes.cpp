#include "pch.h"
#include "LevelRes.h"

namespace ff7r
{
	LevelRes::LevelRes()
		: Resource(RES_TYPE::LEVEL)
	{
	}

	LevelRes::~LevelRes()
	{
	}

	int LevelRes::Load(const wstring& _strFilePath)
	{
		return S_OK;
	}
}