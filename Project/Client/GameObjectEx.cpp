#include "pch.h"
#include "GameObjectEx.h"

#include <Engine\Component.h>
namespace ff7r
{
	void GameObjectEx::FinalTick()
	{
		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::SCRIPT; ++i)
		{
			if (nullptr != GetComponent((COMPONENT_TYPE)i))
				GetComponent((COMPONENT_TYPE)i)->FinalTick();
		}

		const vector<GameObject*>& vecChild = GetChild();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			vecChild[i]->FinalTick();
		}
	}
}