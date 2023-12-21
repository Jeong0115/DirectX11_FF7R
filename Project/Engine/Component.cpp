#include "pch.h"

#include "Component.h"
namespace ff7r
{
	Component::Component(COMPONENT_TYPE _type)
		: owner(nullptr)
		, type(_type)
	{
	}

	Component::Component(const Component& _other)
		: Entity(_other)
		, owner(nullptr)
		, type(_other.type)
	{
	}

	Component::~Component()
	{
	}
}
