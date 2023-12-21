#pragma once
#include "Component.h"
#include "Material.h"
#include "Mesh.h"

#include "ptr.h"

namespace ff7r
{
    class RenderComponent : public Component
    {
    public:
        RenderComponent(COMPONENT_TYPE _type);  
        ~RenderComponent();

        virtual void Render() = 0;

        void SetMesh(Ptr<Mesh> _mesh) { mesh = _mesh; }
        void SetMaterial(Ptr<Material> _mtrl);

        Ptr<Mesh>       GetMesh()           { return mesh; }
        Ptr<Material>   GetMaterial()       { return current_mtrl; }
        Ptr<Material>   GetSharedMaterial() { return shaderd_mtrl; }
        Ptr<Material>   GetDynamicMaterial();

        virtual void SaveToLevelFile(FILE* _file) override;
        virtual void LoadFromLevelFile(FILE* _file) override;

    private:
        Ptr<Mesh>              mesh;
        Ptr<Material>          shaderd_mtrl;  // ���� ���׸���
        Ptr<Material>          dynamic_mtrl; // SharedMaterial ���纻
        Ptr<Material>          current_mtrl; // ���� ��� ���� ����
    };
}
