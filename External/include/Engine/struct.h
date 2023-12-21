#pragma once



struct Vertex
{
	vec3 pos;
	vec4 color;
	vec2 uv;

	vec3 normal;
	vec3 tangent;
	vec3 binormal;
};
typedef Vertex Vtx;

// Event
struct Event
{
	EVENT_TYPE	type;
	DWORD_PTR	w_param;
	DWORD_PTR	l_param;
};

struct ImguiEvent
{
	IMGUI_EVENT_TYPE	event_type;
	RES_TYPE			res_type;
	DWORD_PTR           target;
	string              desc;
};


struct DebugShapeInfo
{
	SHAPE_TYPE	type;
	Matrix		mat_world;
	vec3		world_pos;
	vec3		world_scale;
	vec3		world_rotation;
	vec4		color;
	float		max_time;
	float		cur_time;
};

struct LightColor
{
	vec4 diffuse;	// ���� ����
	vec4 ambient;	// �ֺ� ��(ȯ�� ��)
};

// LightInfo
struct LightInfo
{
	LightColor	light_color;		 // ���� ����

	vec4		world_pos;   // ������ ���� �����̽� ��ġ
	vec4		world_dir;	 // ���� ������ ����

	UINT		light_type;   // ���� Ÿ��(���⼺, ��, ����Ʈ)
	float		radius;		 // ���� �ݰ�(��Ÿ�)
	float		angle;		 // ���� ����
	int			padding;
};


// TileMap
struct Tile
{
	vec2 left_top;
	vec2 slice;
};

// Animator2D
struct Anim2DFrame
{
	vec2	left_top_uv;
	vec2	slice_uv;
	vec2	offset;
	float	duration;
};


// Particle
struct Particle
{
	vec4	local_pos;		// ������Ʈ�κ��� ������ �Ÿ�
	vec4	world_pos;		// ��ƼŬ ���� ������ġ
	vec4	world_scale;	// ��ƼŬ ũ��	
	vec4	color;			// ��ƼŬ ����
	vec4	velocity;		// ��ƼŬ ���� �ӵ�
	vec4	force;			// ��ƼŬ�� �־��� ��
	vec4	rand_force;	// ��ƼŬ�� ����Ǵ� �������� ��

	float   age;			// ���� �ð�
	float   prev_age;		// ���� ������ ���� �ð�
	float   normal_age;	// ������ �����ð��� 0~1�� ����ȭ �� ��
	float	life_time;		// ����
	float	mass;			// ����
	float   scale_factor;	// �߰� ũ�� ����

	int     active;			// ��ƼŬ Ȱ��ȭ ����
	int     padding;
};

struct RWParticleBuffer
{	
	int		spawn_count;			// ���� ��ų ��ƼŬ ����
	int		padding[3];
};


struct ParticleModule
{
	// ���� ���
	vec4    spawn_color;
	vec4	spawn_scale_min;
	vec4	spawn_scale_max;
	vec3	box_scale;	
	float	sphere_radius;	
	int		spawn_shape_type;		// 0 : BOX, 1 : Sphere
	int		spawn_rate;			// �ʴ� ���� ����
	int		space_type;				// ��ƼŬ ������Ʈ ��ǥ�� ( 0 : World,  1 : Local)
	float   life_time_min;		// �ּ� ����
	float   life_time_max;		// �ִ� ����
	int     spawn_padding[3];

	// Color Change ���
	vec4	start_color;		// �ʱ� ����
	vec4	end_color;			// ���� ����

	// Scale Change ���
	float	starct_scale;			// �ʱ� ����
	float	end_scale;			// ���� ����	

	// ���� �ִ�ũ��
	int		particle_count_max;
	int		padding;

	// Add Velocity ���
	vec4	velocity_dir;
	int     velocity_type;	// 0 : From Center, 1: To Center, 2 : Fixed Direction	
	float	offset_angle;		
	float	speed;
	int     velocity_padding;

	// Drag ��� - �ӵ� ����
	float	start_drag;
	float	end_drag;

	// NoiseForce ��� - ���� �� ����	
	float	noise_term;		// ���� �� ���� ����
	float	noise_force;	// ���� �� ũ��

	// Render ���
	int		velocity_alignment;	// 1 : �ӵ����� ���(�̵� �������� ȸ��) 0 : ��� ����
	int		velocity_scale;		// 1 : �ӵ��� ���� ũ�� ��ȭ ���, 0 : ��� ����	
	float   speed_max;			// �ִ� ũ�⿡ �����ϴ� �ӷ�
	vec4	velocity_scale_max;	// �ӷ¿� ���� ũ�� ��ȭ�� �ִ�ġ
	int		render_padding;

	// Module Check
	int		module_check[(UINT)PARTICLE_MODULE::END];
};



// ===================
// ������� ���� ����ü
// ===================
struct TransformData
{
	Matrix mat_world;
	Matrix mat_view;
	Matrix mat_proj;

	Matrix mat_wv;
	Matrix mat_wvp;
};
extern TransformData Tansform_Matrix;



struct MtrlConst
{
	int		array_int[4];
	float	array_float[4];
	vec2	array_vec2[4];
	vec4	array_vec4[4];
	Matrix	array_mat[4];
	int		array_tex[(UINT)TEX_PARAM::TEX_END];
};


struct Global
{
	vec2  resolution;
	float delta_time;
	float accumulated_time;
	UINT  light2d_count;
	UINT  light3d_count;
	int	  global_padding[2];
};
extern Global GlobalData;



