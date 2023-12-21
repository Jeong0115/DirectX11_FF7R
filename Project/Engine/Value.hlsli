
#ifndef _VALUE
#define _VALUE

#include "Struct.hlsli"

cbuffer TRANSFORM : register(b0)
{
    row_major matrix Mat_World;
    row_major matrix Mat_View;
    row_major matrix Mat_Proj;
    
    row_major matrix Mat_WV;
    row_major matrix Mat_WVP;
};

cbuffer MATERIAL : register(b1)
{
    int Int_0;
    int Int_1;
    int Int_2;
    int Int_3;
    
    float Float_0;
    float Float_1;
    float Float_2;
    float Float_3;
    
    float2 Vec2_0;
    float2 Vec2_1;
    float2 Vec2_2;
    float2 Vec2_3;

    float4 Vec4_0;
    float4 Vec4_1;
    
    float4 Vec4_2;
    float4 Vec4_3;
    
    matrix Mat_0;
    matrix Mat_1;
    matrix Mat_2;
    matrix Mat_3;
    
    int BTex_0;
    int BTex_1;
    int BTex_2;
    int BTex_3;
    int BTex_4;
    int BTex_5;
    int BTex_6;
    int BTex_7;
    
    int BCube_0;
    int BCube_1;
    
    int BArr_Tex_0;
    int BArr_Tex_1;
};

cbuffer GLOBAL : register(b2)
{
    float2 Resolution;
    float DT;
    float Acc_Time;
    uint Light2D_Cnt;
    uint Light3D_Cnt;
    int2 Padding;
}

Texture2D Tex_0 : register(t0);
Texture2D Tex_1 : register(t1);
Texture2D Tex_2 : register(t2);
Texture2D Tex_3 : register(t3);
Texture2D Tex_4 : register(t4);
Texture2D Tex_5 : register(t5);
Texture2D Tex_6 : register(t6);
Texture2D Tex_7 : register(t7);

TextureCube Cube_0 : register(t8);
TextureCube Cube_1 : register(t9);

Texture2DArray Arr_Tex_0 : register(t10);
Texture2DArray Arr_Tex_1 : register(t11);

StructuredBuffer<LightInfo> Light2D_Buffer : register(t12);
StructuredBuffer<LightInfo> Light3D_Buffer : register(t13);


SamplerState Sampler_0 : register(s0);
SamplerState Sampler_1 : register(s1);
 
#define PI 3.1415926535f

#endif