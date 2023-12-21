#pragma once

#include "ptr.h"

#include "ParticleShader.h"
#include "RenderComponent.h"

namespace ff7r
{
    class StructuredBuffer;
    class ParticleSystem : public RenderComponent
    {
    public:
        ParticleSystem();
        ~ParticleSystem();

        CLONE(ParticleSystem);

        virtual void FinalTick() override;
        virtual void Render() override;

        virtual void SaveToLevelFile(FILE* _file) override;
        virtual void LoadFromLevelFile(FILE* _file) override;

    private:
        StructuredBuffer*   particle_buff;
        StructuredBuffer*   rw_buff;
        StructuredBuffer*   module_buff;

        ParticleModule      module_data;
        Ptr<ParticleShader> particle_shader;

        float               accumulated_time;
    };
}
