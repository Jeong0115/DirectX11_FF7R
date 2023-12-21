#include "pch.h"
#include "CScriptMgr.h"

#include "CameraMoveScript.h"
#include "CScriptMgr.h"
#include "func.h"
#include "MonsterScript.h"
#include "PlayerScript.h"
#include "TestScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CameraMoveScript");
	_vec.push_back(L"CScriptMgr");
	_vec.push_back(L"func");
	_vec.push_back(L"MonsterScript");
	_vec.push_back(L"PlayerScript");
	_vec.push_back(L"TestScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CameraMoveScript" == _strScriptName)
		return new CameraMoveScript;
	if (L"CScriptMgr" == _strScriptName)
		return new CScriptMgr;
	if (L"func" == _strScriptName)
		return new func;
	if (L"MonsterScript" == _strScriptName)
		return new MonsterScript;
	if (L"PlayerScript" == _strScriptName)
		return new PlayerScript;
	if (L"TestScript" == _strScriptName)
		return new TestScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::AMERAMOVESCRIPT:
		return new CameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::SCRIPTMGR:
		return new CScriptMgr;
		break;
	case (UINT)SCRIPT_TYPE::UNC:
		return new func;
		break;
	case (UINT)SCRIPT_TYPE::ONSTERSCRIPT:
		return new MonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::LAYERSCRIPT:
		return new PlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::ESTSCRIPT:
		return new TestScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::AMERAMOVESCRIPT:
		return L"CameraMoveScript";
		break;

	case SCRIPT_TYPE::SCRIPTMGR:
		return L"CScriptMgr";
		break;

	case SCRIPT_TYPE::UNC:
		return L"func";
		break;

	case SCRIPT_TYPE::ONSTERSCRIPT:
		return L"MonsterScript";
		break;

	case SCRIPT_TYPE::LAYERSCRIPT:
		return L"PlayerScript";
		break;

	case SCRIPT_TYPE::ESTSCRIPT:
		return L"TestScript";
		break;

	}
	return nullptr;
}