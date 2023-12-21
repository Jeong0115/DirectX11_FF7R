#include "pch.h"

#include "Collider2D.h"
#include "GameObject.h"
#include "Layer.h"
#include "Level.h"
#include "LevelMgr.h"

#include "CollisionMgr.h"
namespace ff7r
{
	CollisionMgr::CollisionMgr()
		: matrix{}
		, map_colli_id{}
	{
	}

	CollisionMgr::~CollisionMgr()
	{
	}

	void CollisionMgr::Tick()
	{
		Level* level = LevelMgr::GetInst()->GetCurLevel();

		for (UINT row = 0; row < MAX_LAYER; ++row)
		{
			for (UINT col = row; col < MAX_LAYER; ++col)
			{
				if (!(matrix[row] & (1 << col)))
					continue;

				// iRow ���̾�� iCol ���̾�� ���� �浹�˻縦 �����Ѵ�.
				CollisionBtwLayer(level->GetLayer(row), level->GetLayer(col));
			}
		}
	}

	void CollisionMgr::CollisionBtwLayer(Layer* _left_layer, Layer* _right_layer)
	{
		const vector<GameObject*>& left_objects = _left_layer->GetObjects();
		const vector<GameObject*>& right_objects = _right_layer->GetObjects();

		if (_left_layer == _right_layer)
		{
			for (size_t i = 0; i < left_objects.size(); ++i)
			{
				for (size_t j = i + 1; j < right_objects.size(); ++j)
				{
					CollisionBtwObject(left_objects[i], right_objects[j]);
				}
			}
		}
		else
		{
			for (size_t i = 0; i < left_objects.size(); ++i)
			{
				for (size_t j = 0; j < right_objects.size(); ++j)
				{
					CollisionBtwObject(left_objects[i], right_objects[j]);
				}
			}
		}
	}

	void CollisionMgr::CollisionBtwObject(GameObject* _left_object, GameObject* right_object)
	{
		if (!(_left_object->GetCollider2D() && right_object->GetCollider2D()))
			return;
		
		// �浹ü ID ����
		CollisionID id = {};
		id.left_id = _left_object->GetCollider2D()->GetID();
		id.right_id = right_object->GetCollider2D()->GetID();

		// ID �˻�
		map<UINT_PTR, bool>::iterator iter = map_colli_id.find(id.id);
		if (iter == map_colli_id.end())
		{
			map_colli_id.insert(make_pair(id.id, false));
			iter = map_colli_id.find(id.id);
		}

		// �� �� �ϳ��� ���� ���� ���¶��(Dead ����)
		bool is_dead = false;
		if (_left_object->IsDead() || right_object->IsDead())
		{
			is_dead = true;
		}

		// �� �浹ü�� ���� �浹 ������ Ȯ��
		if (CollisionBtwCollider(_left_object->GetCollider2D(), right_object->GetCollider2D()))
		{
			// ������ �浹�� ���� �ְ�, ���� �ϳ� �̻��� ���� �����̶��
			if (is_dead && iter->second)
			{
				_left_object->GetCollider2D()->EndOverlap(right_object->GetCollider2D());
				right_object->GetCollider2D()->EndOverlap(_left_object->GetCollider2D());
			}
			else if (iter->second)
			{
				// ��� �浹 ��
				_left_object->GetCollider2D()->OnOverlap(right_object->GetCollider2D());
				right_object->GetCollider2D()->OnOverlap(_left_object->GetCollider2D());
			}
			else
			{
				// �̹� �����ӿ� �浹
				if (!is_dead) // ���� �ϳ��� Dead ���¸� �浹�� �����Ѵ�.
				{
					_left_object->GetCollider2D()->BeginOverlap(right_object->GetCollider2D());
					right_object->GetCollider2D()->BeginOverlap(_left_object->GetCollider2D());
					iter->second = true;
				}
			}
		}

		else
		{
			// �浹 ����
			if (iter->second)
			{
				_left_object->GetCollider2D()->EndOverlap(right_object->GetCollider2D());
				right_object->GetCollider2D()->EndOverlap(_left_object->GetCollider2D());
				iter->second = false;
			}
		}


	}

	// �� �浹ü�� �浹 �˻� ����
	bool CollisionMgr::CollisionBtwCollider(Collider2D* _left_colli, Collider2D* _right_colli)
	{
		// 0 -- 1
		// |    |
		// 3 -- 2
		vec3 local_pos[4] =
		{
			vec3(-0.5f, 0.5f, 0.f),
			vec3(0.5f, 0.5f, 0.f),
			vec3(0.5f, -0.5f, 0.f),
			vec3(-0.5f, -0.5f, 0.f),
		};

		// �� �浹ü�� �� ǥ�� ���� 2���� ����
		vec3 arrProj[4] = {};

		arrProj[0] = XMVector3TransformCoord(local_pos[1], _left_colli->GetColliderWorldMat()) - XMVector3TransformCoord(local_pos[0], _left_colli->GetColliderWorldMat());
		arrProj[1] = XMVector3TransformCoord(local_pos[3], _left_colli->GetColliderWorldMat()) - XMVector3TransformCoord(local_pos[0], _left_colli->GetColliderWorldMat());

		arrProj[2] = XMVector3TransformCoord(local_pos[1], _right_colli->GetColliderWorldMat()) - XMVector3TransformCoord(local_pos[0], _right_colli->GetColliderWorldMat());
		arrProj[3] = XMVector3TransformCoord(local_pos[3], _right_colli->GetColliderWorldMat()) - XMVector3TransformCoord(local_pos[0], _right_colli->GetColliderWorldMat());

		// �� �浹ü�� �߽����� ����
		vec3 center = XMVector3TransformCoord(vec3(0.f, 0.f, 0.f), _right_colli->GetColliderWorldMat()) - XMVector3TransformCoord(vec3(0.f, 0.f, 0.f), _left_colli->GetColliderWorldMat());


		// �и��� �׽�Ʈ
		for (int i = 0; i < 4; ++i)
		{
			vec3 vProj = arrProj[i];
			vProj.Normalize();

			// 4���� ǥ����͸� ������ ���������� ������Ų �Ÿ��� ���� / 2
			float fProjDist = 0.f;
			for (int j = 0; j < 4; ++j)
			{
				fProjDist += fabsf(arrProj[j].Dot(vProj));
			}
			fProjDist /= 2.f;

			float fCenter = fabsf(center.Dot(vProj));

			if (fProjDist < fCenter)
				return false;
		}


		return true;
	}



	void CollisionMgr::LayerCheck(UINT _left, UINT _right)
	{
		UINT row = (UINT)_left;
		UINT col = (UINT)_right;

		if (row > col)
		{
			UINT temp = col;
			col = row;
			row = temp;
		}

		matrix[row] |= (1 << col);
	}

	void CollisionMgr::LayerCheck(const wstring& _left_layer_name, const wstring& _right_layer_name)
	{
		Level* level = LevelMgr::GetInst()->GetCurLevel();

		Layer* left = level->FindLayerByName(_left_layer_name);
		Layer* right = level->FindLayerByName(_right_layer_name);

		LayerCheck(left->GetLayerIndex(), right->GetLayerIndex());
	}
}