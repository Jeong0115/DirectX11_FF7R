#pragma once
#include "ptr.h"

#include "ComputeShader.h"

namespace ff7r
{
    class Texture;
    class SetColorShader : public ComputeShader
    {
    public:
        SetColorShader(UINT _thread_x_per_group, UINT _thread_y_per_group, UINT _thread_z_per_group);
        ~SetColorShader();

        void SetTargetTexture(Ptr<Texture> _tex) { target_tex = _tex; }
        void SetColor(vec3 _rgb) { mtrl_const_data.array_vec4[0] = _rgb; }

        virtual void UpdateData() override;
        virtual void Clear() override;

    private:
        Ptr<Texture>   target_tex;
    };
}
