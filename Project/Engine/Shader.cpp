#include "pch.h"

#include "Shader.h"
namespace ff7r
{
	Shader::Shader(RES_TYPE _type)
		: Resource(_type, true)
		, error_blob{}
	{
	}

	Shader::~Shader()
	{
	}
}
