#pragma once

namespace ff7r
{
	class Layer;
	class Collider2D;

	union CollisionID
	{
		struct
		{
			UINT left_id;
			UINT right_id;
		};

		UINT_PTR id;
	};

	class CollisionMgr : public Singleton<CollisionMgr>
	{
	public:
		void Tick();

		void LayerCheck(UINT _left, UINT _right);
		void LayerCheck(const wstring& _left_layer_name, const wstring& _right_layer_name);

		void Clear() { memset(matrix, 0, sizeof(UINT) * MAX_LAYER); }	

	private:
		SINGLE(CollisionMgr);

		void CollisionBtwLayer(Layer* _left_layer, Layer* _right_layer);
		void CollisionBtwObject(GameObject* _left_object, GameObject* right_object);
		bool CollisionBtwCollider(Collider2D* _left_colli, Collider2D* _right_colli);

		UINT					matrix[MAX_LAYER];
		map<UINT_PTR, bool>		map_colli_id;
	};

}