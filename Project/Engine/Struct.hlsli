#ifndef _STRUCT
#define _STRUCT

struct LightColor
{
    float4 diffuse; // ���� ����
    float4 ambient; // �ֺ� ��(ȯ�� ��)
};

// LightInfo
struct LightInfo
{
    LightColor color; // ���� ����
    
    float4  world_pos; // ������ ���� �����̽� ��ġ
    float4  world_dir; // ���� ������ ����
    
    uint    light_type; // ���� Ÿ��(���⼺, ��, ����Ʈ)
    float   radius; // ���� �ݰ�(��Ÿ�)
    float   angle; // ���� ����    
    int     padding;
};


// Particle
struct Particle
{
    float4 local_pos;
    float4 world_pos; // ��ƼŬ ��ġ
    float4 world_scale; // ��ƼŬ ũ��
    float4 color; // ��ƼŬ ����
    float4 velocity; // ��ƼŬ ���� �ӵ�
    float4 force; // ��ƼŬ�� �־��� ��
    float4 random_force; // ��ƼŬ�� ����Ǵ� ���� ��

    float age; // ���� �ð�
    float prev_age; // ���� ������ �����ð�
    float normalize_age; // ������ �����ð��� 0~1�� ����ȭ �� ��
    float life_time; // ����
    float mass; // ����
    float scale_factor; // �߰� ũ�� ����
    
    int active;
    int padding;
};


struct tParticleModule
{
	// ���� ���
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

	// Color Change ���
    float4 vStartColor; // �ʱ� ����
    float4 vEndColor; // ���� ����

	// Scale Change ���
    float StartScale; // �ʱ� ũ��
    float EndScale; // ���� ũ��	

    int iMaxParticleCount;
    int ipad;
    
    // Add Velocity ���
    float4 vVelocityDir;
    int AddVelocityType; // 0 : From Center, 1 : Fixed Direction	
    float OffsetAngle;
    float Speed;
    int addvpad;
    
    // Drag ���
    float StartDrag;
    float EndDrag;
    
    // NoiseForce ���
    float fNoiseTerm;
    float fNoiseForce;
        
    // Render ���
    int VelocityAlignment; // 1 : �ӵ����� ���(�̵� �������� ȸ��) 0 : ��� ����
    int VelocityScale; // 1 : �ӵ��� ���� ũ�� ��ȭ ���, 0 : ��� ����	
    float vMaxSpeed; // �ִ� ũ�⿡ �����ϴ� �ӷ�
    float4 vMaxVelocityScale; // �ӷ¿� ���� ũ�� ��ȭ�� �ִ�ġ
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