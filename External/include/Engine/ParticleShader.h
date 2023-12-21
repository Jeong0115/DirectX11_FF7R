#pragma once
#include "ptr.h"

#include "ComputeShader.h"
#include "Texture.h"

namespace ff7r
{
    class StructuredBuffer;
    class ParticleShader : public ComputeShader
    {
    public:
        ParticleShader(UINT _thread_x_per_group, UINT _thread_y_per_group, UINT _thread_z_per_group);
        ~ParticleShader();

        virtual void UpdateData() override;
        virtual void Clear() override;

        void SetParticleBuffer(StructuredBuffer* _buff);
        void SetRWParticleBuffer(StructuredBuffer* _buff)   { rw_buff = _buff; }
        void SetModuleData(StructuredBuffer* _buff)         { module_buff = _buff; }
        void SetParticleObjectPos(vec3 _pos)                { mtrl_const_data.array_vec4[0] = _pos; }

        void SetNoiseTexture(Ptr<Texture> _tex)
        {
            noise_tex = _tex;
            mtrl_const_data.array_vec2[0] = vec2(noise_tex->GetWidth(), noise_tex->GetHeight());
        }


    private:
        StructuredBuffer*   particle_buff;
        StructuredBuffer*   rw_buff;
        StructuredBuffer*   module_buff;
        Ptr<Texture>        noise_tex;
    };
}