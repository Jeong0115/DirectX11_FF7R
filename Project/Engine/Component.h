#pragma once
#include "Entity.h"
#include "GameObject.h"

#define GET_OTHER_COMPONENT(Type) Type* Get##Type() {return owner->Get##Type();}
namespace ff7r
{
    class Transform;
    class MeshRender;
    class Component : public Entity
    {
    public:
        Component(COMPONENT_TYPE _type);
        Component(const Component& _other);
        virtual ~Component();

        virtual void Begin() {}
        virtual void Tick() {}
        virtual void FinalTick() = 0;
        virtual Component* Clone() = 0;

        COMPONENT_TYPE GetType() { return type; }
        GameObject* GetOwner() { return owner; }

        virtual void SaveToLevelFile(FILE* _file) = 0;
        virtual void LoadFromLevelFile(FILE* _file) = 0;

        GET_OTHER_COMPONENT(Transform);
        GET_OTHER_COMPONENT(MeshRender);
        GET_OTHER_COMPONENT(Camera);
        GET_OTHER_COMPONENT(Collider2D);
        GET_OTHER_COMPONENT(Light2D);
        GET_OTHER_COMPONENT(Light3D);
        GET_OTHER_COMPONENT(TileMap);
        GET_OTHER_COMPONENT(Animator2D);

    private:
        GameObject*             owner;
        const COMPONENT_TYPE    type;

        friend class GameObject;
    };
}
