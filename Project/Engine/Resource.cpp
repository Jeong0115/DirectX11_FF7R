#include "pch.h"
#include "Resource.h"

namespace ff7r
{
	Resource::Resource(RES_TYPE _type, bool _engine_resource)
		: type(_type)
		, ref_count(0)
		, engine_resource(_engine_resource)
		, key{}
		, relative_path{}
	{
	}

	Resource::Resource(const Resource& _other)
		: Entity(_other)
		, type(_other.type)
		, ref_count(0)
		, key(_other.key)
		, relative_path(_other.relative_path)
	{
	}

	Resource::~Resource()
	{
	}

	void Resource::Release()
	{
		--ref_count;

		if (0 == ref_count)
		{
			delete this;
		}
	}
}