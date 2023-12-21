#ifndef _FUNC
#define _FUNC

#include "Value.hlsli"

void CalcLight2D(float3 _world_pos, inout LightColor _light_color)
{
    for (int i = 0; i < Light2D_Cnt; ++i)
    {
        if (Light2D_Buffer[i].light_type == 0)
        {
            _light_color.diffuse.rgb += Light2D_Buffer[i].color.diffuse.rgb;
            _light_color.ambient.rgb += Light2D_Buffer[i].color.ambient.rgb;
        }
        else if (Light2D_Buffer[i].light_type == 1)
        {
            float3 light_world_pos = float3(Light2D_Buffer[i].world_pos.xy, 0.f);
            float3 world_pos = float3(_world_pos.xy, 0.f);

            float dist = abs(distance(world_pos, light_world_pos));
            float power = saturate(1.f - (dist / Light2D_Buffer[i].radius));
        
            _light_color.diffuse.rgb += Light2D_Buffer[i].color.diffuse.rgb * power;
        }
        else if (Light2D_Buffer[i].light_type == 2)
        {
            
        }
    }
}

void CalcLight2D(float3 _world_pos, float3 _world_dir, inout LightColor _light_color)
{
    for (int i = 0; i < Light2D_Cnt; ++i)
    {
        if (Light2D_Buffer[i].light_type == 0)
        {
            float diffuse_pow = saturate(dot(-Light2D_Buffer[i].world_dir.xyz, _world_dir));
            _light_color.diffuse.rgb += Light2D_Buffer[i].color.diffuse.rgb * diffuse_pow;
            _light_color.ambient.rgb += Light2D_Buffer[i].color.ambient.rgb;
        }
        else if (Light2D_Buffer[i].light_type == 1)
        {
            float3 light_world_pos = float3(Light2D_Buffer[i].world_pos.xy, 0.f);
            float3 world_pos = float3(_world_pos.xy, 0.f);

            // 광원 중심에서 물체를 향하는 방향
            float3 light = normalize(world_pos - light_world_pos);
            float diffuse_pow = saturate(dot(-light, _world_dir));
            
            float dist = abs(distance(world_pos, light_world_pos));
            float power = saturate(1.f - (dist / Light2D_Buffer[i].radius));
        
            _light_color.diffuse.rgb += Light2D_Buffer[i].color.diffuse.rgb * diffuse_pow * power;
        }
        else if (Light2D_Buffer[i].light_type == 2)
        {
            
        }
    }
}

void CalcLight3D(float3 _view_pos, float3 _view_normal, int _light_idx, inout LightColor _light_color, inout float _spec_pow)
{
    LightInfo light_info = Light3D_Buffer[_light_idx];
    
    float light_pow = 0.f;
    float spec_pow = 0.f;
    float3 view_light_dir = (float3) 0.f;
        
    // DirLight 인 경우
    if (0 == light_info.light_type)
    {
        // ViewSpace 에서의 광원의 방향
        view_light_dir = normalize(mul(float4(normalize(light_info.world_dir.xyz), 0.f), Mat_View)).xyz;
        
        // ViewSpace 에서의 노말벡터와 광원의 방향을 내적 (램버트 코사인 법칙)    
        light_pow = saturate(dot(_view_normal, -view_light_dir));
        
        // 반사광
        float3 view_reflect = normalize(view_light_dir + 2.f * (dot(-view_light_dir, _view_normal)) * _view_normal);
        float3 eye = normalize(_view_pos);
        
        // 반사광의 세기 구하기
        spec_pow = saturate(dot(view_reflect, -eye));
        spec_pow = pow(spec_pow, 20);
    }
    
    // Point Light 인 경우
    else if (1 == light_info.light_type)
    {
        float dist_pow = 1.f;
        
        // ViewSpace 에서의 광원의 위치
        float3 view_light_pos = mul(float4(light_info.world_pos.xyz, 1.f), Mat_View).xyz;
        
        // 광원으로부터 오는 빛의 방향 구하기
        view_light_dir = normalize(_view_pos - view_light_pos);
        
        // 포인트 라이트로부터 거리체크
        float dist = distance(_view_pos, view_light_pos);
        dist_pow = saturate(1.f - (dist / light_info.radius));
               
        // ViewSpace 에서의 노말벡터와 광원의 방향을 내적 (램버트 코사인 법칙)    
        light_pow = saturate(dot(_view_normal, -view_light_dir)) * dist_pow;
        
        // 반사광
        float3 view_reflect = normalize(view_light_dir + 2.f * (dot(-view_light_dir, _view_normal)) * _view_normal);
        float3 eye = normalize(_view_pos);
        //reflect()
        // 반사광의 세기 구하기
        spec_pow = saturate(dot(view_reflect, -eye));
        spec_pow = pow(spec_pow, 20) * dist_pow;
    }
    
    else
    {
        //거리에 따른 빛 세기
        float dist_pow = 1.f;
        
        //// 빛의 기본 방향
        //float3 light_dir = normalize(mul(float4(light_info.world_dir.xyz, 1.0f), Mat_View)).xyz;
        
        //// 빛과 오브젝트 사이의 방향
        //float3 view_light_pos = mul(float4(light_info.world_pos.xyz, 1.f), Mat_View).xyz;
        //view_light_dir = normalize(_view_pos - view_light_pos);
        
        //// 내적을 계산합니다.
        //float angle = acos(dot(light_dir, view_light_dir));
        
        //if(angle < light_info.angle)
        //    light_pow = 1;
        
        //float dist = distance(_view_pos, view_light_pos);
        //dist_pow = saturate(1.f - (dist / light_info.radius));
        
        //float3 view_reflect = normalize(view_light_dir + 2.f * (dot(-view_light_dir, _view_normal)) * _view_normal);
        //float3 eye = normalize(_view_pos);
        
        //spec_pow = saturate(dot(view_reflect, -eye));
        //spec_pow = pow(spec_pow, 20) * dist_pow;
        
        
        float3 light_dir = normalize(mul(float4(light_info.world_dir.xyz, 0.0f), Mat_View)).xyz;

        // 빛의 위치 (뷰 공간)
        float3 view_light_pos = mul(float4(light_info.world_pos.xyz, 1.f), Mat_View).xyz;
        view_light_dir = normalize(view_light_pos - _view_pos);

        // 내적을 계산하여 빛의 각도와 비교
        float cosAngle = dot(light_dir, -view_light_dir); // 빛의 방향과 반대 방향으로 내적
        if (cosAngle < cos(light_info.angle))
        {
            light_pow = 0; // 빛이 오브젝트에 도달하지 않음
        }
        else
        {
            // 빛과 오브젝트 표면 노말 간의 관계 계산
            float ndotl = saturate(dot(_view_normal, view_light_dir));
            if (ndotl > 0)
            {
                light_pow = ndotl; // 빛이 오브젝트 앞면에 도달
            }
            else
            {
                light_pow = 0; // 빛이 오브젝트 뒷면에 도달하지 않음
            }
        }

        // 거리에 따른 감쇠 계산
        //float dist = distance(_view_pos, view_light_pos);
        //dist_pow = saturate(1.f - (dist / light_info.radius));

        float dist = distance(_view_pos, view_light_pos);
        dist_pow = saturate(cos((dist / light_info.radius) * (PI / 2.f)));
        
        // 반사 벡터 계산
        float3 view_reflect = normalize(reflect(-view_light_dir, _view_normal));
        float3 eye = normalize(-_view_pos); // 뷰어의 방향

        // Specular 계산
        spec_pow = saturate(dot(view_reflect, eye));
        spec_pow = pow(spec_pow, 20) * dist_pow;
        
        light_pow = saturate(light_pow * dist_pow);

    }
  
    
    // 결과 전달하기
    _light_color.diffuse += light_info.color.diffuse * light_pow;
    _light_color.ambient += light_info.color.ambient;
    _spec_pow += spec_pow;
}

// ======
// Random
// ======
static float GaussianFilter[5][5] =
{
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.0219f, 0.0983f, 0.1621f, 0.0983f, 0.0219f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
};

void GaussianSample(in Texture2D _NoiseTex, float2 _vResolution, float _NomalizedThreadID, out float3 _vOut)
{
    float2 vUV = float2(_NomalizedThreadID, 0.5f);
    
    vUV.x += Acc_Time * 0.5f;
    
    // sin 그래프로 텍스쳐의 샘플링 위치 UV 를 계산
    vUV.y -= (sin((_NomalizedThreadID - (Acc_Time /*그래프 우측 이동 속도*/)) * 2.f * 3.1415926535f * 10.f /*반복주기*/) / 2.f);
    
    if (1.f < vUV.x)
        vUV.x = frac(vUV.x);
    else if (vUV.x < 0.f)
        vUV.x = 1.f + frac(vUV.x);
    
    if (1.f < vUV.y)
        vUV.y = frac(vUV.y);
    else if (vUV.y < 0.f)
        vUV.y = 1.f + frac(vUV.y);
        
    int2 pixel = vUV * _vResolution;
    int2 offset = int2(-2, -2);
    float3 vOut = (float3) 0.f;
    
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            vOut += _NoiseTex[pixel + offset + int2(j, i)] * GaussianFilter[i][j];
        }
    }
    
    _vOut = vOut;
}

#endif