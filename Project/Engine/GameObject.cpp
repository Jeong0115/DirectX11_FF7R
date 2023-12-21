#include "pch.h"

#include "Component.h"
#include "Level.h"
#include "LevelMgr.h"
#include "Layer.h"
#include "MeshRender.h"
#include "Script.h"

#include "GameObject.h"
namespace ff7r
{
	GameObject::GameObject()
		: components{}
		, render_component(nullptr)
		, parent(nullptr)
		, layer_idx(-1)
		, is_dead(false)
		, life_time(0.f)
		, cur_life_time(0.f)
		, is_life_span(false)
	{
	}

	GameObject::GameObject(const GameObject& _other)
		: Entity(_other)
		, components{}
		, render_component(nullptr)
		, parent(nullptr)
		, layer_idx(-1)
		, is_dead(false)
		, life_time(0.f)
		, cur_life_time(0.f)
		, is_life_span(false)
	{
		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (_other.components[i])
			{
				AddComponent(_other.components[i]->Clone());
			}
		}

		for (size_t i = 0; i < _other.scripts.size(); ++i)
		{
			AddComponent(_other.scripts[i]->Clone());
		}

		for (size_t i = 0; i < _other.child.size(); ++i)
		{
			AddChild(_other.child[i]->Clone());
		}
	}

	GameObject::~GameObject()
	{
		Safe_Del_Array(components);
		Safe_Del_Vec(scripts);
		Safe_Del_Vec(child);
	}

	void GameObject::Begin()
	{
		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (nullptr != components[i])
				components[i]->Begin();
		}

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->Begin();
		}

		for (size_t i = 0; i < child.size(); ++i)
		{
			child[i]->Begin();
		}
	}

	void GameObject::Tick()
	{
		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (nullptr != components[i])
				components[i]->Tick();
		}

		for (size_t i = 0; i < scripts.size(); ++i)
		{
			scripts[i]->Tick();
		}

		for (size_t i = 0; i < child.size(); ++i)
		{
			child[i]->Tick();
		}
	}

	void GameObject::FinalTick()
	{
		if (is_life_span)
		{
			cur_life_time += DT;
			if (life_time < cur_life_time)
			{
				DestroyObject(this);
			}
		}

		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::SCRIPT; ++i)
		{
			if (nullptr != components[i])
				components[i]->FinalTick();
		}

		for (size_t i = 0; i < child.size(); ++i)
		{
			child[i]->FinalTick();
		}

		// �Ҽ� ���̾ ���µ� finaltick �� ȣ��Ǿ���.
		assert(-1 != layer_idx);

		// ���̾� ���
		Layer* cur_layer = LevelMgr::GetInst()->GetCurLevel()->GetLayer(layer_idx);
		cur_layer->RegisterObject(this);
	}

	void GameObject::Render()
	{
		if (nullptr != render_component)
			render_component->Render();
	}

	void GameObject::AddComponent(Component* _component)
	{
		_component->owner = this;

		// ������Ʈ�� ��ũ��Ʈ�� ���
		if (COMPONENT_TYPE::SCRIPT == _component->GetType())
		{
			scripts.push_back((Script*)_component);
		}

		// ��ũ��Ʈ�� ������ �Ϲ� ������Ʈ�� ���
		else
		{
			// �̹� �����ϰ� �ִ� ������Ʈ�� ���
			assert(!components[(UINT)_component->GetType()]);

			components[(UINT)_component->GetType()] = _component;

			// RenderComponent Ȯ��
			if (COMPONENT_TYPE::MESHRENDER <= _component->GetType()
				&& _component->GetType() <= COMPONENT_TYPE::DECAL)
			{
				assert(!render_component);
				render_component = (RenderComponent*)_component;
			}
		}
	}

	void GameObject::AddChild(GameObject* _object)
	{
		if (_object->parent)
		{
			// ���� �θ� ������ ���� ���� �� ����
			_object->DisconnectFromParent();
		}

		else
		{
			// ���� �θ� ������, �Ҽ� ���̾�� �ֻ����θ� ��Ͽ��� ���ŵ� �� ����
			_object->ChangeToChildType();
		}

		// �θ� �ڽ� ����
		_object->parent = this;
		child.push_back(_object);
	}

	bool GameObject::IsAncestor(GameObject* _target)
	{
		GameObject* _parent = parent;
		while (_parent)
		{
			if (_parent == _target)
			{
				return true;
			}
			_parent = _parent->parent;
		}

		return false;
	}

	void GameObject::DisconnectFromParent()
	{
		if (!parent)
			return;

		vector<GameObject*>::iterator iter = parent->child.begin();
		for (; iter != parent->child.end(); ++iter)
		{
			if (this == *iter)
			{
				parent->child.erase(iter);
				parent = nullptr;
				return;
			}
		}

		assert(nullptr);
	}

	void GameObject::ChangeToChildType()
	{
		assert(-1 <= layer_idx && layer_idx < MAX_LAYER);

		if (-1 != layer_idx)
		{
			Layer* cur_layer = LevelMgr::GetInst()->GetCurLevel()->GetLayer(layer_idx);
			cur_layer->RemoveFromParentList(this);
		}
	}

	void GameObject::AddParentList()
	{
		Layer* cur_layer = LevelMgr::GetInst()->GetCurLevel()->GetLayer(layer_idx);
		cur_layer->AddParentList(this);
	}
}