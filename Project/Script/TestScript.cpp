#include "pch.h"

#include "TestScript.h"
namespace ff7r
{
	TestScript::TestScript()
		: Script((UINT)SCRIPT_TYPE::TESTSCRIPT)
	{
	}

	TestScript::~TestScript()
	{
	}


	void TestScript::Begin()
	{
	}

	void TestScript::Tick()
	{
	}

	void TestScript::SaveToLevelFile(FILE* _File)
	{
	}

	void TestScript::LoadFromLevelFile(FILE* _FILE)
	{
	}
}