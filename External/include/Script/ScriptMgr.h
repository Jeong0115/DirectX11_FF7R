#pragma once

#include <vector>
#include <string>

namespace ff7r
{
	enum SCRIPT_TYPE
	{
		CAMERAMOVESCRIPT,
		MONSTERSCRIPT,
		PLAYERSCRIPT,
		TESTSCRIPT,
		END,
	};

	using namespace std;

	class Script;
	class ScriptMgr
	{
	public:
		static void GetScriptInfo(vector<wstring>& _result);
		static Script* GetScript(const wstring& _script_name);
		static Script* GetScript(UINT _script_type);
		static const wchar_t* GetScriptName(Script* _pScript);
	};
}