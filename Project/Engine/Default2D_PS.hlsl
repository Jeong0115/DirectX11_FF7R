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

float4 main(VSOut _in) : SV_TARGET
{
    float4 out_color = (float4) 0.f;
        
    if (BTex_0)
    {
        out_color = Tex_0.Sample(Sampler_0, _in.uv);
    }
    else
    {
        out_color = float4(1.f, 0.f, 1.f, 1.f);
    }

    //if (0.f == vOutColor.a)
    //    discard; // «»ºø Ω¶¿Ã¥ı ¡ﬂ¥‹
    
    if (Int_2 == 1)
        out_color = float4(1.f, 0.f, 0.f, 1.f);
    
    return out_color;
}