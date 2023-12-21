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
	vec4 diffuse;	// 빛의 색상
	vec4 ambient;	// 주변 광(환경 광)
};

// LightInfo
struct LightInfo
{
	LightColor	light_color;		 // 빛의 색상

	vec4		world_pos;   // 광원의 월드 스페이스 위치
	vec4		world_dir;	 // 빛을 보내는 방향

	UINT		light_type;   // 빛의 타입(방향성, 점, 스포트)
	float		radius;		 // 빛의 반경(사거리)
	float		angle;		 // 빛의 각도
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
	vec4	local_pos;		// 오브젝트로부터 떨어진 거리
	vec4	world_pos;		// 파티클 최종 월드위치
	vec4	world_scale;	// 파티클 크기	
	vec4	color;			// 파티클 색상
	vec4	velocity;		// 파티클 현재 속도
	vec4	force;			// 파티클에 주어진 힘
	vec4	rand_force;	// 파티클에 적용되는 랜덤방향 힘

	float   age;			// 생존 시간
	float   prev_age;		// 이전 프레임 생존 시간
	float   normal_age;	// 수명대비 생존시간을 0~1로 정규화 한 값
	float	life_time;		// 수명
	float	mass;			// 질량
	float   scale_factor;	// 추가 크기 배율

	int     active;			// 파티클 활성화 여부
	int     padding;
};

struct RWParticleBuffer
{	
	int		spawn_count;			// 스폰 시킬 파티클 개수
	int		padding[3];
};


struct ParticleModule
{
	// 스폰 모듈
	vec4    spawn_color;
	vec4	spawn_scale_min;
	vec4	spawn_scale_max;
	vec3	box_scale;	
	float	sphere_radius;	
	int		spawn_shape_type;		// 0 : BOX, 1 : Sphere
	int		spawn_rate;			// 초당 생성 개수
	int		space_type;				// 파티클 업데이트 좌표계 ( 0 : World,  1 : Local)
	float   life_time_min;		// 최소 수명
	float   life_time_max;		// 최대 수명
	int     spawn_padding[3];

	// Color Change 모듈
	vec4	start_color;		// 초기 색상
	vec4	end_color;			// 최종 색상

	// Scale Change 모듈
	float	starct_scale;			// 초기 배율
	float	end_scale;			// 최종 배율	

	// 버퍼 최대크기
	int		particle_count_max;
	int		padding;

	// Add Velocity 모듈
	vec4	velocity_dir;
	int     velocity_type;	// 0 : From Center, 1: To Center, 2 : Fixed Direction	
	float	offset_angle;		
	float	speed;
	int     velocity_padding;

	// Drag 모듈 - 속도 제한
	float	start_drag;
	float	end_drag;

	// NoiseForce 모듈 - 랜덤 힘 적용	
	float	noise_term;		// 랜덤 힘 변경 간격
	float	noise_force;	// 랜덤 힘 크기

	// Render 모듈
	int		velocity_alignment;	// 1 : 속도정렬 사용(이동 방향으로 회전) 0 : 사용 안함
	int		velocity_scale;		// 1 : 속도에 따른 크기 변화 사용, 0 : 사용 안함	
	float   speed_max;			// 최대 크기에 도달하는 속력
	vec4	velocity_scale_max;	// 속력에 따른 크기 변화량 최대치
	int		render_padding;

	// Module Check
	int		module_check[(UINT)PARTICLE_MODULE::END];
};



// ===================
// 상수버퍼 대응 구조체
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



