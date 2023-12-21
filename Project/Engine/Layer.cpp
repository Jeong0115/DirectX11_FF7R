#include "pch.h"

#include "Layer.h"

#include "GameObject.h"
namespace ff7r
{
	Layer::Layer()
	{
	}

	Layer::~Layer()
	{
		Safe_Del_Vec(parent_objects);
	}

	void Layer::Begin()
	{
		for (size_t i = 0; i < parent_objects.size(); ++i)
		{
			parent_objects[i]->Begin();
		}
	}

	void Layer::Tick()
	{
		for (size_t i = 0; i < parent_objects.size(); ++i)
		{
			parent_objects[i]->Tick();
		}
	}

	void Layer::FinalTick()
	{
		vector<GameObject*>::iterator iter = parent_objects.begin();
		for (; iter != parent_objects.end(); )
		{
			(*iter)->FinalTick();

			if ((*iter)->IsDead())
			{
				iter = parent_objects.erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}

	void Layer::AddGameObject(GameObject* _object, bool _follow_parent)
	{
		parent_objects.push_back(_object);

		// 소유하고 있는 모든 자식오브젝트가 있는지 검사
		static list<GameObject*> queue;
		queue.clear();

		queue.push_back(_object);

		while (!queue.empty())
		{
			GameObject* object = queue.front();
			queue.pop_front();

			for (size_t i = 0; i < object->child.size(); ++i)
			{
				queue.push_back(object->child[i]);
			}

			// 부모타입 or 소속 레이어가 없는경우 or 부모와 같이 이동하는 경우
			if (nullptr == object->parent || -1 == object->layer_idx || _follow_parent)
				object->layer_idx = layer_idx;
		}
	}


	void Layer::RemoveFromParentList(GameObject* _object)
	{
		vector<GameObject*>::iterator iter = parent_objects.begin();

		for (; iter != parent_objects.end(); ++iter)
		{
			if ((*iter) == _object)
			{
				parent_objects.erase(iter);
				return;
			}
		}

		assert(nullptr);
	}

	void Layer::AddParentList(GameObject* _object)
	{
		assert(!_object->parent);
		parent_objects.push_back(_object);
	}
}