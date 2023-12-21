#pragma once

#include "Component.h"

namespace ff7r
{
	class Light3D : public Component
	{
    public:
        Light3D();
        ~Light3D();

        CLONE(Light3D);

        virtual void FinalTick() override;

        virtual void SaveToLevelFile(FILE* _file) override;
        virtual void LoadFromLevelFile(FILE* _file) override;

        void SetLightType(LIGHT_TYPE _type) { light_info.light_type = (UINT)_type; }
        void SetLightDirection(vec3 _dir) { light_info.world_dir = _dir; }
        void SetLightDiffuse(vec3 _diffuse) { light_info.light_color.diffuse = _diffuse; }
        void SetLightAmbient(vec3 _ambient) { light_info.light_color.ambient = _ambient; }
        void SetRadius(float _radius) { light_info.radius = _radius; }
        void SetAngle(float _angle) { light_info.angle = _angle; }

    private:
        LightInfo      light_info;
	};
}

