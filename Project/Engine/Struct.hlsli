#ifndef _STRUCT
#define _STRUCT

struct LightColor
{
    float4 diffuse; // 빛의 색상
    float4 ambient; // 주변 광(환경 광)
};

// LightInfo
struct LightInfo
{
    LightColor color; // 빛의 색상
    
    float4  world_pos; // 광원의 월드 스페이스 위치
    float4  world_dir; // 빛을 보내는 방향
    
    uint    light_type; // 빛의 타입(방향성, 점, 스포트)
    float   radius; // 빛의 반경(사거리)
    float   angle; // 빛의 각도    
    int     padding;
};


// Particle
struct Particle
{
    float4 local_pos;
    float4 world_pos; // 파티클 위치
    float4 world_scale; // 파티클 크기
    float4 color; // 파티클 색상
    float4 velocity; // 파티클 현재 속도
    float4 force; // 파티클에 주어진 힘
    float4 random_force; // 파티클에 적용되는 랜덤 힘

    float age; // 생존 시간
    float prev_age; // 이전 프레임 생존시간
    float normalize_age; // 수명대비 생존시간을 0~1로 정규화 한 값
    float life_time; // 수명
    float mass; // 질량
    float scale_factor; // 추가 크기 배율
    
    int active;
    int padding;
};


struct tParticleModule
{
	// 스폰 모듈
    float4 vSpawnColor;
    float4 vSpawnScaleMin;
    float4 vSpawnScaleMax;
    float3 vBoxShapeScale;
    float fSphereShapeRadius;
    int SpawnShapeType; // Sphere , Box
    int SpawnRate;
    int Space; // 0 World, 1 Local    
    float MinLifeTime;
    float MaxLifeTime;
    int3 spawnpad;

	// Color Change 모듈
    float4 vStartColor; // 초기 색상
    float4 vEndColor; // 최종 색상

	// Scale Change 모듈
    float StartScale; // 초기 크기
    float EndScale; // 최종 크기	

    int iMaxParticleCount;
    int ipad;
    
    // Add Velocity 모듈
    float4 vVelocityDir;
    int AddVelocityType; // 0 : From Center, 1 : Fixed Direction	
    float OffsetAngle;
    float Speed;
    int addvpad;
    
    // Drag 모듈
    float StartDrag;
    float EndDrag;
    
    // NoiseForce 모듈
    float fNoiseTerm;
    float fNoiseForce;
        
    // Render 모듈
    int VelocityAlignment; // 1 : 속도정렬 사용(이동 방향으로 회전) 0 : 사용 안함
    int VelocityScale; // 1 : 속도에 따른 크기 변화 사용, 0 : 사용 안함	
    float vMaxSpeed; // 최대 크기에 도달하는 속력
    float4 vMaxVelocityScale; // 속력에 따른 크기 변화량 최대치
    int renderpad;

    
    // Module Check
    int Spawn;
    int ColorChange;
    int ScaleChange;
    int AddVelocity;
    
    int Drag;
    int NoiseForce;
    int Render;
    int modulepad;
};

#endif