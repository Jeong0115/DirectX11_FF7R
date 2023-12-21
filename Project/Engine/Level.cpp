#include "pch.h"

#include "GameObject.h"
#include "Layer.h"
#include "RenderMgr.h"

#include "Level.h"
namespace ff7r
{
	Level::Level()
		: layers{}
		, state(LEVEL_STATE::STOP)
	{
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			layers[i] = new Layer;
			layers[i]->layer_idx = i;
		}
	}

	Level::~Level()
	{
		Safe_Del_Array(layers);
	}

	void Level::Begin()
	{
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			layers[i]->Begin();
		}
	}

	void Level::Tick()
	{
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			layers[i]->Tick();
		}
	}

	void Level::FinalTick()
	{
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			layers[i]->FinalTick();
		}
	}

	Layer* Level::FindLayerByName(const wstring& _name)
	{
		for (int i = 0; i < MAX_LAYER; ++i)
		{
			if (layers[i]->GetName() == _name)
				return layers[i];
		}

		return nullptr;
	}


	void Level::AddGameObject(GameObject* _object, int _layer_idx, bool _follow_parent)
	{
		layers[_layer_idx]->AddGameObject(_object, _follow_parent);
	}

	void Level::AddGameObject(GameObject* _object, const wstring& _layer_name, bool _follow_parent)
	{
		Layer* layer = FindLayerByName(_layer_name);
		assert(layer);

		layer->AddGameObject(_object, _follow_parent);
	}

	void Level::ChangeState(LEVEL_STATE _state)
	{
		state = _state;

		if (LEVEL_STATE::PLAY == state)
		{
			RenderMgr::GetInst()->SetRenderFunc(true);
			Begin();
		}
		else
		{
			RenderMgr::GetInst()->SetRenderFunc(false);
		}
	}


	void Level::Clear()
	{
		for (int i = 0; i < MAX_LAYER; ++i)
		{
			layers[i]->objects.clear();
		}
	}


	GameObject* Level::FindObjectByName(const wstring& _name)
	{
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			const vector<GameObject*>& objects = layers[i]->GetObjects();
			for (size_t j = 0; j < objects.size(); ++j)
			{
				if (_name == objects[j]->GetName())
					return objects[j];
			}
		}

		return nullptr;
	}

	void Level::FindObjectByName(const wstring& _name, vector<GameObject*>& _result)
	{
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			const vector<GameObject*>& objects = layers[i]->GetObjects();
			for (size_t j = 0; j < objects.size(); ++j)
			{
				if (_name == objects[j]->GetName())
					_result.push_back(objects[j]);
			}
		}
	}
}