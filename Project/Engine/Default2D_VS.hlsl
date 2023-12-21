#include "Value.hlsli"

struct VSIn
{
    float3 pos : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct VSOut
{
    float4 pos : SV_Position;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

VSOut main(VSIn _in)
{
    VSOut output = (VSOut) 0.0f;
    
    output.pos = mul(float4(_in.pos, 1.f), Mat_WVP);
    output.color = _in.color;
    output.uv = _in.uv;
    
    return output;
}