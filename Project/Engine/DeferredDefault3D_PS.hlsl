#include "Value.hlsli"
#include "Func.hlsli"

struct VSOut
{
    float4 pos      : SV_Position;
    float4 color    : COLOR;
    float2 uv       : TEXCOORD;
    
    float3 view_pos         : POSITION;
    float3 view_normal      : NORMAL;
    float3 view_tangent     : TANGENT;
    float3 view_binormal    : BINORMAL;
};

struct PS_OUT
{
    float4 diffuse  : SV_Target0;
    float4 normal   : SV_Target1;
    float4 position : SV_Target2;
    float4 data     : SV_Target3;
};

PS_OUT main(VSOut _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    output.diffuse = float4(1.f, 1.f, 1.f, 1.f);
    
    float3 view_normal = _in.view_normal;
    
    if (BTex_0)
    {
        output.diffuse = Tex_0.Sample(Sampler_0, _in.uv);
        output.diffuse.a = 1.0f;
    }
    
    if (BTex_1)
    {
        float3 normal = Tex_1.Sample(Sampler_0, _in.uv).xyz;
        normal = normal * 2.0f - 1.0f;
        
        float3x3 mat_rotate = { _in.view_tangent, -_in.view_binormal, _in.view_normal };
        view_normal = mul(normal, mat_rotate);
    }
    
         
    output.normal = float4(view_normal, 1.f);
    output.position = float4(_in.view_pos, 1.f);
    output.data = float4(0.f, 0.f, 0.f, 1.f);
    
    return output;
}