#include "pch.h"
#include "Entity.h"

namespace ff7r
{
	UINT Entity::Next_ID = 0;

	Entity::Entity()
		: id(Next_ID++)
		, name(L"Default")
	{
	}

	Entity::Entity(const Entity& _other)
		: name(_other.name)
		, id(Next_ID++)
	{
	}

	Entity::~Entity()
	{
	}
}
