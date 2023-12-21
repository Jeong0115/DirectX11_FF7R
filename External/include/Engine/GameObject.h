#pragma once
#include "Entity.h"

namespace ff7r
{
#define GET_COMPONENT(Type, TYPE) Type* Get##Type() const { return (Type*)components[(UINT)COMPONENT_TYPE::TYPE]; }

    class Component;
    class Transform;
    class MeshRender;
    class Collider2D;
    class Animator2D;
    class Light2D;
    class Light3D;
    class Camera;
    class RenderComponent;
    class TileMap;
    class SkyBox;
    class Script;

    class GameObject : public Entity
    {
    public:
        GameObject();
        GameObject(const GameObject& _other);
        ~GameObject();

        void            Begin();
        virtual void    Tick();
        virtual void    FinalTick();
        virtual void    Render();

        void AddComponent(Component* _component);
        void AddChild(GameObject* _object);

        Component* GetComponent(COMPONENT_TYPE _comp_type) { return components[(UINT)_comp_type]; }
        const vector<GameObject*>& GetChild() { return child; }
        GameObject* GetParent() const { return parent; }
        RenderComponent* GetRenderComponent() const { return render_component; }

        int GetLayerIndex() { return layer_idx; }
        void SetLayerIndex(int index) { layer_idx = index; }    
        
        bool IsDead() { return is_dead; }
        bool IsAncestor(GameObject* _Target);
        void SetLifeSpan(float _fTime)
        {
            if (is_life_span)
                return;

            life_time = _fTime;
            is_life_span = true;
        }

        const vector<Script*>& GetScripts() { return scripts; }
        template<typename T> T* GetScript();

        GET_COMPONENT(Transform,    TRANSFORM);
        GET_COMPONENT(MeshRender,   MESHRENDER);
        GET_COMPONENT(Camera,       CAMERA);
        GET_COMPONENT(Collider2D,   COLLIDER2D);
        GET_COMPONENT(Light2D,      LIGHT2D);
        GET_COMPONENT(Light3D,      LIGHT3D);
        GET_COMPONENT(TileMap,      TILEMAP);
        GET_COMPONENT(Animator2D, ANIMATOR2D);
        GET_COMPONENT(SkyBox,       SKYBOX);

    private:
        CLONE(GameObject)

        void DisconnectFromParent();
        void ChangeToChildType();
        void AddParentList();

        GameObject*             parent;
        Component*              components[(UINT)COMPONENT_TYPE::END];
        RenderComponent*        render_component;
        vector<Script*>         scripts;
        vector<GameObject*>     child;

        float                   life_time;
        float                   cur_life_time;
        int                     layer_idx;
        bool                    is_dead;
        bool                    is_life_span;

        friend class Layer;
        friend class EventMgr;
    };

    template<typename T>
    inline T* GameObject::GetScript()
    {
        for (size_t i = 0; i < scripts.size(); ++i)
        {
            T* script = dynamic_cast<T*> (scripts[i]);
            if (nullptr != script)
                return script;
        }

        return nullptr;
    }
}
