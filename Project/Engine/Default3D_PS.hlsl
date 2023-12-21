#include "Value.hlsli"
#include "Func.hlsli"

struct VSOut
{
    float4 pos : SV_Position;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    
    float3 view_pos : POSITION;
    float3 view_normal : NORMAL;
    float3 view_tangent : TANGENT;
    float3 view_binormal : BINORMAL;
};

#define SPEC_COEFF Float_0

float4 main(VSOut _in) : SV_TARGET
{
    float4 out_color = float4(0.5f, 0.5f, 0.5f, 1.f);
    float3 view_normal = _in.view_normal;
    
    if (BTex_0)
    {
        out_color = Tex_0.Sample(Sampler_0, _in.uv);
    }
    else
    {
        return _in.color;
    }
    
    if (BTex_1)
    {
        float3 normal = Tex_1.Sample(Sampler_0, _in.uv).xyz;
        normal = normal * 2.0f - 1.0f;
        
        float3x3 mat_rotate = { _in.view_tangent, -_in.view_binormal, _in.view_normal };
        view_normal = mul(normal, mat_rotate);
    }
    
    LightColor light_color = (LightColor) 0.0f;
    float spec_pow = 0.0f;
    
    for (int i = 0; i < Light3D_Cnt; ++i)
    {
        CalcLight3D(_in.view_pos, view_normal, i, light_color, spec_pow);
    }
                 
    out_color.xyz = out_color.xyz * light_color.diffuse.xyz
                    + out_color.xyz * light_color.ambient.xyz
                    + saturate(Light3D_Buffer[0].color.diffuse.xyz) * 0.3f * spec_pow * SPEC_COEFF;
    
    return out_color;
}