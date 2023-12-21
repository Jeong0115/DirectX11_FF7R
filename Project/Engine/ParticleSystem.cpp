#include "pch.h"

#include "Device.h"
#include "Material.h"
#include "Mesh.h"
#include "ResourceMgr.h"
#include "StructuredBuffer.h"
#include "TimeMgr.h"
#include "Transform.h"

#include "ParticleSystem.h"
namespace ff7r
{
	ParticleSystem::ParticleSystem()
		: RenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
		, particle_buff(nullptr)
		, rw_buff(nullptr)
		, module_data{}
		, accumulated_time(0.f)
	{
		module_data.particle_count_max = 2000;

		module_data.module_check[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN] = true;
		module_data.spawn_rate = 20;
		module_data.spawn_color = vec3(0.4f, 1.f, 0.4f);
		module_data.spawn_scale_min = vec3(15.f, 15.f, 1.f);
		module_data.spawn_scale_max = vec3(20.f, 20.f, 1.f);

		module_data.spawn_shape_type = 0;
		module_data.box_scale = vec3(200.f, 200.f, 200.f);
		module_data.space_type = 0; // �ùķ��̼� ��ǥ��

		module_data.life_time_min = 3.f;
		module_data.life_time_max = 5.f;

		module_data.module_check[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = false;
		module_data.starct_scale = 1.5f;
		module_data.end_scale = 0.2f;

		module_data.module_check[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = true;
		module_data.start_color = vec3(0.2f, 0.3f, 1.0f);
		module_data.end_color = vec3(0.4f, 1.f, 0.4f);

		module_data.module_check[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
		module_data.velocity_type = 0; // From Center
		module_data.speed = 700.f;
		module_data.velocity_dir;
		module_data.offset_angle;

		module_data.module_check[(UINT)PARTICLE_MODULE::DRAG] = true;
		module_data.start_drag = 500.f;
		module_data.end_drag = -500.f;

		module_data.module_check[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
		module_data.noise_term = 0.3f;
		module_data.noise_force = 50.f;

		module_data.module_check[(UINT)PARTICLE_MODULE::RENDER] = true;
		module_data.velocity_alignment = true;
		module_data.velocity_scale = true;
		module_data.velocity_scale_max = vec3(15.f, 1.f, 1.f);
		module_data.speed_max = 500.f;


		// ���� �޽�
		SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"PointMesh"));

		// ��ƼŬ ���� ����
		SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"ParticleRenderMtrl"));

		// ��ƼŬ ������Ʈ ��ǻƮ ���̴�	
		particle_shader = (ParticleShader*)ResourceMgr::GetInst()->FindRes<ComputeShader>(L"ParticleUpdateCS").Get();

		particle_buff = new StructuredBuffer;
		particle_buff->Create(sizeof(Particle), module_data.particle_count_max, SB_TYPE::READ_WRITE, false);

		rw_buff = new StructuredBuffer;
		rw_buff->Create(sizeof(RWParticleBuffer), 1, SB_TYPE::READ_WRITE, true);

		module_buff = new StructuredBuffer;
		module_buff->Create(sizeof(ParticleModule), 1, SB_TYPE::READ_ONLY, true);
	}

	ParticleSystem::~ParticleSystem()
	{
		if (nullptr != particle_buff)
			delete particle_buff;

		if (nullptr != rw_buff)
			delete rw_buff;

		if (nullptr != module_buff)
			delete module_buff;
	}


	void ParticleSystem::FinalTick()
	{
		// ���� ����Ʈ ���
		// 1�� ���� �ð�
		float spawn_time = 1.f / (float)module_data.spawn_rate;
		accumulated_time += DT;

		// �����ð��� ���� �����ð��� �Ѿ��
		if (spawn_time < accumulated_time)
		{
			// �ʰ� ���� ==> ���� ����
			float remainder = accumulated_time / spawn_time;

			// �������� ���� �ð�
			accumulated_time = spawn_time * (remainder - floor(remainder));

			// ���ۿ� ���� ī��Ʈ ����
			RWParticleBuffer rwbuffer = { (int)remainder, };
			rw_buff->SetData(&rwbuffer);
		}


		// ��ƼŬ ������Ʈ ��ǻƮ ���̴�
		module_buff->SetData(&module_data);

		particle_shader->SetParticleBuffer(particle_buff);
		particle_shader->SetRWParticleBuffer(rw_buff);
		particle_shader->SetModuleData(module_buff);
		particle_shader->SetNoiseTexture(ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\noise\\noise_01.png"));
		particle_shader->SetParticleObjectPos(GetTransform()->GetWorldPos());

		particle_shader->Execute();
	}

	void ParticleSystem::Render()
	{
		GetTransform()->UpdateData();

		// ��ƼŬ���� t20 �� ���ε�
		particle_buff->UpdateData(20, PIPELINE_STAGE::PS_ALL);

		// ��� ������ t21 �� ���ε�
		module_buff->UpdateData(21, PIPELINE_STAGE::PS_GEOMETRY);

		// Particle Render	
		Ptr<Texture> pParticleTex = ResourceMgr::GetInst()->Load<Texture>(L"Particle_0", L"texture\\particle\\AlphaCircle.png");
		GetMaterial()->SetTexParam(TEX_0, pParticleTex);

		GetMaterial()->UpdateData();
		GetMesh()->RenderParticle(module_data.particle_count_max);

		// ��ƼŬ ���� ���ε� ����
		particle_buff->Clear();
		module_buff->Clear();
	}

	void ParticleSystem::SaveToLevelFile(FILE* _file)
	{
		RenderComponent::SaveToLevelFile(_file);

		fwrite(&module_data, sizeof(ParticleModule), 1, _file);
		SaveResRef(particle_shader.Get(), _file);
	}

	void ParticleSystem::LoadFromLevelFile(FILE* _file)
	{
		RenderComponent::LoadFromLevelFile(_file);

		fread(&module_data, sizeof(ParticleModule), 1, _file);

		int i = 0;
		fread(&i, sizeof(i), 1, _file);

		if (i)
		{
			wstring key, relative_path;
			LoadWString(key, _file);
			LoadWString(relative_path, _file);

			particle_shader = (ParticleShader*)ResourceMgr::GetInst()->FindRes<ComputeShader>(key).Get();
		}
	}
}