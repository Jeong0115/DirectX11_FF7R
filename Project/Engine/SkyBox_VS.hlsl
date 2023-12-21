#include "Value.hlsli"

struct VSIn
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VSOut
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
    
    float3 dir : POSITION;
};

VSOut main(VSIn _in)
{
    VSOut output = (VSOut) 0.0f;
    
    float4 pos = mul(mul(float4(_in.pos, 0.f), Mat_View), Mat_Proj);
    pos.z = pos.w;
        
    output.pos = pos;
    output.uv = _in.uv;
    output.dir = _in.pos;

    
    return output;
}