#pragma once
#include "Entity.h"

namespace ff7r
{
    class GameObject;
    class Layer : public Entity
    {
    public:
        Layer();
        ~Layer();

        void Begin();
        void Tick();
        void FinalTick();

        int GetLayerIndex() { return layer_idx; }
        const vector<GameObject*>& GetParentObject() { return parent_objects; }
        const vector<GameObject*>& GetObjects() { return objects; }

    private:
        CLONE(Layer)

        void AddGameObject(GameObject* _object, bool _follow_parent);
        void RegisterObject(GameObject* _object) { objects.push_back(_object); }
        void RemoveFromParentList(GameObject* _object);
        void AddParentList(GameObject* _object);

        vector<GameObject*> parent_objects; 
        vector<GameObject*> objects;    
        int                 layer_idx;   

        friend class Level;
        friend class GameObject;
        friend class EventMgr;
    };
}

