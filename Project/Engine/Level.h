#pragma once
#include "Entity.h"

namespace ff7r
{
    class Layer;
    class GameObject;
    class Level : public Entity
    {
    public:
        Level();
        ~Level();

        void Begin();
        void Tick();
        void FinalTick();

        Layer* FindLayerByName(const wstring& _name);
        Layer* GetLayer(int _layer_idx) { assert(!(_layer_idx < 0)); return layers[_layer_idx]; }

        GameObject* FindObjectByName(const wstring& _name);
        void FindObjectByName(const wstring& _name, vector<GameObject*>& _result);

        void AddGameObject(GameObject* _object, int _layer_idx, bool _follow_parent);
        void AddGameObject(GameObject* _object, const wstring& _layer_name, bool _follow_parent);

        void ChangeState(LEVEL_STATE _State);
        LEVEL_STATE GetState() { return state; }

    private:
        CLONE(Level);

        void Clear();

        Layer*          layers[MAX_LAYER];
        LEVEL_STATE     state;

        friend class LevelMgr;
    };
}
