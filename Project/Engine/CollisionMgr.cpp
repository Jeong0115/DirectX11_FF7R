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

				// iRow 레이어와 iCol 레이어는 서로 충돌검사를 진행한다.
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
		
		// 충돌체 ID 생성
		CollisionID id = {};
		id.left_id = _left_object->GetCollider2D()->GetID();
		id.right_id = right_object->GetCollider2D()->GetID();

		// ID 검색
		map<UINT_PTR, bool>::iterator iter = map_colli_id.find(id.id);
		if (iter == map_colli_id.end())
		{
			map_colli_id.insert(make_pair(id.id, false));
			iter = map_colli_id.find(id.id);
		}

		// 둘 중 하나라도 삭제 예정 상태라면(Dead 상태)
		bool is_dead = false;
		if (_left_object->IsDead() || right_object->IsDead())
		{
			is_dead = true;
		}

		// 두 충돌체가 지금 충돌 중인지 확인
		if (CollisionBtwCollider(_left_object->GetCollider2D(), right_object->GetCollider2D()))
		{
			// 이전에 충돌한 적이 있고, 둘중 하나 이상이 삭제 예정이라면
			if (is_dead && iter->second)
			{
				_left_object->GetCollider2D()->EndOverlap(right_object->GetCollider2D());
				right_object->GetCollider2D()->EndOverlap(_left_object->GetCollider2D());
			}
			else if (iter->second)
			{
				// 계속 충돌 중
				_left_object->GetCollider2D()->OnOverlap(right_object->GetCollider2D());
				right_object->GetCollider2D()->OnOverlap(_left_object->GetCollider2D());
			}
			else
			{
				// 이번 프레임에 충돌
				if (!is_dead) // 둘중 하나라도 Dead 상태면 충돌을 무시한다.
				{
					_left_object->GetCollider2D()->BeginOverlap(right_object->GetCollider2D());
					right_object->GetCollider2D()->BeginOverlap(_left_object->GetCollider2D());
					iter->second = true;
				}
			}
		}

		else
		{
			// 충돌 해제
			if (iter->second)
			{
				_left_object->GetCollider2D()->EndOverlap(right_object->GetCollider2D());
				right_object->GetCollider2D()->EndOverlap(_left_object->GetCollider2D());
				iter->second = false;
			}
		}


	}

	// 두 충돌체의 충돌 검사 진행
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

		// 두 충돌체의 각 표면 벡터 2개씩 구함
		vec3 arrProj[4] = {};

		arrProj[0] = XMVector3TransformCoord(local_pos[1], _left_colli->GetColliderWorldMat()) - XMVector3TransformCoord(local_pos[0], _left_colli->GetColliderWorldMat());
		arrProj[1] = XMVector3TransformCoord(local_pos[3], _left_colli->GetColliderWorldMat()) - XMVector3TransformCoord(local_pos[0], _left_colli->GetColliderWorldMat());

		arrProj[2] = XMVector3TransformCoord(local_pos[1], _right_colli->GetColliderWorldMat()) - XMVector3TransformCoord(local_pos[0], _right_colli->GetColliderWorldMat());
		arrProj[3] = XMVector3TransformCoord(local_pos[3], _right_colli->GetColliderWorldMat()) - XMVector3TransformCoord(local_pos[0], _right_colli->GetColliderWorldMat());

		// 두 충돌체의 중심점을 구함
		vec3 center = XMVector3TransformCoord(vec3(0.f, 0.f, 0.f), _right_colli->GetColliderWorldMat()) - XMVector3TransformCoord(vec3(0.f, 0.f, 0.f), _left_colli->GetColliderWorldMat());


		// 분리축 테스트
		for (int i = 0; i < 4; ++i)
		{
			vec3 vProj = arrProj[i];
			vProj.Normalize();

			// 4개의 표면백터를 지정된 투영축으로 투영시킨 거리의 총합 / 2
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