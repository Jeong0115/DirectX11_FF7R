#include "pch.h"
#include "Script.h"
#include "Collider2D.h"

namespace ff7r
{
	Script::Script(UINT _script_type)
		: Component(COMPONENT_TYPE::SCRIPT)
		, script_type(_script_type)
		, parameters{}
	{
	}

	Script::~Script()
	{
	}

	void Script::AddScriptParam(SCRIPT_PARAM _param, void* _data, const string& _desc)
	{
		parameters.push_back(ScriptParam{ _param , _data , _desc });
	}
}