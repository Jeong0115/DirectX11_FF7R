#include "pch.h"

#include "GameObject.h"
#include "Level.h"
#include "LevelMgr.h"
#include "RenderMgr.h"
#include "ResourceMgr.h"

#include "EventMgr.h"
namespace ff7r
{
	EventMgr::EventMgr()
		: is_level_changed(false)
	{
	}

	EventMgr::~EventMgr()
	{
	}

	void EventMgr::Tick()
	{
		is_level_changed = false;

		DeleteObject();

		for (size_t i = 0; i < events.size(); ++i)
		{
			switch (events[i].type)
			{
				// wParam : GameObject, lParam : Layer Index
			case EVENT_TYPE::CREATE_OBJECT:
			{
				GameObject* object = (GameObject*)events[i].w_param;
				int layer_idx = (int)events[i].l_param;

				LevelMgr::GetInst()->GetCurLevel()->AddGameObject(object, layer_idx, false);
				if (LevelMgr::GetInst()->GetCurLevel()->GetState() == LEVEL_STATE::PLAY)
				{
					object->Begin();
				}

				is_level_changed = true;
			}
			break;

			case EVENT_TYPE::DELETE_OBJECT:
			{
				GameObject* object = (GameObject*)events[i].w_param;

				if (false == object->is_dead)
				{
					object->is_dead = true;
					delete_objects.push_back(object);
				}
			}
			break;

			case EVENT_TYPE::ADD_CHILD:
				// wParam : ParentObject, lParam : ChildObject
			{
				GameObject* dest_obj = (GameObject*)events[i].w_param;
				GameObject* src_obj = (GameObject*)events[i].l_param;

				// �θ�� ������ ������Ʈ�� ������, Child ������Ʈ�� �ֻ��� �θ� ������Ʈ�� �ȴ�.
				if (nullptr == dest_obj)
				{
					if (src_obj->GetParent())
					{
						// ���� �θ���� ���� ����
						src_obj->DisconnectFromParent();
						// �ֻ��� �θ� ������Ʈ��, �Ҽ� ���̾ ���
						src_obj->AddParentList();
					}
				}
				else
				{
					dest_obj->AddChild(src_obj);
				}

				is_level_changed = true;
			}
			break;

			case EVENT_TYPE::DELETE_RESOURCE:
				// wParam : RES_TYPE, lParam : Resource Adress
			{
				RES_TYPE type = (RES_TYPE)events[i].w_param;
				Resource* res = (Resource*)events[i].l_param;
				ResourceMgr::GetInst()->DeleteRes(type, res->GetKey());
			}
			break;

			case EVENT_TYPE::LEVEL_CHANGE:
			{
				// wParam : Level Adress
				Level* level = (Level*)events[i].w_param;
				LevelMgr::GetInst()->ChangeLevel(level);
				RenderMgr::GetInst()->ClearCamera();
				is_level_changed = true;
			}
			break;
			}
		}
		events.clear();
	}

	void EventMgr::DeleteObject()
	{
		for (size_t i = 0; i < delete_objects.size(); ++i)
		{
			if (nullptr != delete_objects[i])
			{
				// �ڽ� Ÿ�� ������Ʈ�� ���
				if (delete_objects[i]->GetParent())
					delete_objects[i]->DisconnectFromParent();

				delete delete_objects[i];

				is_level_changed = true;
			}
		}
		delete_objects.clear();
	}
}