#include "pch.h"
#include "ScriptMgr.h"
#include "Engine/Script.h"
#include "CameraMoveScript.h"
#include "MonsterScript.h"
#include "PlayerScript.h"
#include "TestScript.h"

namespace ff7r
{
	void ScriptMgr::GetScriptInfo(vector<wstring>& _result)
	{
		_result.push_back(L"CameraMoveScript");
		_result.push_back(L"MonsterScript");
		_result.push_back(L"PlayerScript");
		_result.push_back(L"TestScript");
	}

	Script* ScriptMgr::GetScript(const wstring& _script_name)
	{
		if (L"CameraMoveScript" == _script_name)
			return new CameraMoveScript;
		if (L"MonsterScript" == _script_name)
			return new MonsterScript;
		if (L"PlayerScript" == _script_name)
			return new PlayerScript;
		if (L"TestScript" == _script_name)
			return new TestScript;
		return nullptr;
	}

	Script* ScriptMgr::GetScript(UINT _script_type)
	{
		switch (_script_type)
		{
		case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
			return new CameraMoveScript;
			break;
		case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
			return new MonsterScript;
			break;
		case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
			return new PlayerScript;
			break;
		case (UINT)SCRIPT_TYPE::TESTSCRIPT:
			return new TestScript;
			break;
		}
		return nullptr;
	}

	const wchar_t* ScriptMgr::GetScriptName(Script* _script_name)
	{
		switch ((SCRIPT_TYPE)_script_name->GetScriptType())
		{
		case SCRIPT_TYPE::CAMERAMOVESCRIPT:
			return L"CameraMoveScript";
			break;

		case SCRIPT_TYPE::MONSTERSCRIPT:
			return L"MonsterScript";
			break;

		case SCRIPT_TYPE::PLAYERSCRIPT:
			return L"PlayerScript";
			break;

		case SCRIPT_TYPE::TESTSCRIPT:
			return L"TestScript";
			break;

		}
		return nullptr;
	}
}