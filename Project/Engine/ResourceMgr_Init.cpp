#include "pch.h"

#include "PathMgr.h"
#include "ParticleShader.h"
#include "SetColorShader.h"

#include "ResourceMgr.h"

namespace ff7r
{
	void ResourceMgr::InitSound()
	{
		FMOD::System_Create(&Sound::System);

		if (nullptr == Sound::System)
		{
			assert(nullptr);
		}

		// 32개 채널 생성
		Sound::System->init(32, FMOD_DEFAULT, nullptr);
	}

	void ResourceMgr::CreateNewResource(wstring resName, RES_TYPE resType)
	{
		wstring strPath = PathMgr::GetInst()->GetContentPath();
		strPath = strPath + L"Level\\" + resName + L".lv";

		FILE* pFile = nullptr;

		_wfopen_s(&pFile, strPath.c_str(), L"wb");

		//if (nullptr == pFile)
		//	return E_FAIL;

		// 레벨 이름 저장
		SaveWString(resName, pFile);

		fclose(pFile);
	}

	void ResourceMgr::CreatMeshFbx_Test(void* _VtxSysMem, UINT _iVtxCount, void* _IdxSysMem, UINT _IdxCount, wstring meshName)
	{
		Ptr<Mesh> pMesh = nullptr;
		
		pMesh = new Mesh(true);
		//pMesh->SetTest();
		pMesh->Create(_VtxSysMem, _iVtxCount, _IdxSysMem, _IdxCount);
		
		AddRes(meshName, pMesh);
	}

	void ResourceMgr::CreateDefaultMesh()
	{
		Ptr<Mesh> pMesh = nullptr;

		vector<Vtx> vecVtx;
		vector<UINT> vecIdx;
		Vtx v;



		// ==============
		// PointMesh 생성
		// ==============
		v.pos = vec3(0.f, 0.f, 0.f);
		v.uv = vec2(0.f, 0.f);
		v.color = vec4(1.f, 1.f, 1.f, 1.f);

		UINT idx = 0;

		pMesh = new Mesh(true);
		pMesh->Create(&v, 1, &idx, 1);
		AddRes(L"PointMesh", pMesh);


		// =============
		// RectMesh 생성
		// =============
		// 0 --- 1 
		// |  \  |
		// 3 --- 2
		v.pos = vec3(-0.5f, 0.5f, 0.f);
		v.color = vec4(1.f, 0.f, 0.f, 1.f);
		v.uv = vec2(0.f, 0.f);

		v.normal = vec3(0.f, 0.f, -1.f);
		v.tangent = vec3(1.f, 0.f, 0.f);
		v.binormal = vec3(0.f, -1.f, 0.f);


		vecVtx.push_back(v);

		v.pos = vec3(0.5f, 0.5f, 0.f);
		v.color = vec4(0.f, 1.f, 0.f, 1.f);
		v.uv = vec2(1.f, 0.f);
		vecVtx.push_back(v);

		v.pos = vec3(0.5f, -0.5f, 0.f);
		v.color = vec4(0.f, 0.f, 1.f, 1.f);
		v.uv = vec2(1.f, 1.f);
		vecVtx.push_back(v);

		v.pos = vec3(-0.5f, -0.5f, 0.f);
		v.color = vec4(0.f, 0.f, 0.f, 1.f);
		v.uv = vec2(0.f, 1.f);
		vecVtx.push_back(v);

		vecIdx.push_back(0);
		vecIdx.push_back(2);
		vecIdx.push_back(3);

		vecIdx.push_back(0);
		vecIdx.push_back(1);
		vecIdx.push_back(2);

		pMesh = new Mesh(true);
		pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
		AddRes(L"RectMesh", pMesh);

		vecIdx.clear();
		vecIdx.push_back(0);
		vecIdx.push_back(1);
		vecIdx.push_back(2);
		vecIdx.push_back(3);
		vecIdx.push_back(0);

		pMesh = new Mesh(true);
		pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
		AddRes(L"RectMesh_Debug", pMesh);

		vecVtx.clear();
		vecIdx.clear();


		// ===========
		// Circle Mesh
		// ===========
		//      3 
		//   4     2
		//  5 --0-- 1	
		// 반지름
		float fRadius = 0.5f;

		// 각도
		UINT Slice = 40;
		float fTheta = XM_2PI / (float)Slice;

		// 중심점
		v.pos = vec3(0.f, 0.f, 0.f);
		v.color = vec4(1.f, 1.f, 1.f, 1.f);
		v.uv = vec2(0.5f, 0.5f);
		vecVtx.push_back(v);

		// 정점 위치 지정
		for (UINT i = 0; i < Slice; ++i)
		{
			v.pos = vec3(fRadius * cosf(fTheta * (float)i), fRadius * sinf(fTheta * (float)i), 0.f);
			v.uv = vec2(v.pos.x + 0.5f, -v.pos.y + 0.5f);
			vecVtx.push_back(v);
		}

		// 인덱스 설정
		for (UINT i = 0; i < Slice - 1; ++i)
		{
			vecIdx.push_back(0);
			vecIdx.push_back(i + 2);
			vecIdx.push_back(i + 1);
		}

		// 마지막 삼각형
		vecIdx.push_back(0);
		vecIdx.push_back(1);
		vecIdx.push_back(Slice);

		pMesh = new Mesh(true);
		pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
		AddRes(L"CircleMesh", pMesh);

		vecIdx.clear();
		for (UINT i = 0; i < Slice; ++i)
		{
			vecIdx.push_back(i + 1);
		}
		vecIdx.push_back(1);

		pMesh = new Mesh(true);
		pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
		AddRes(L"CircleMesh_Debug", pMesh);

		vecVtx.clear();
		vecIdx.clear();

		// ========
	// CubeMesh
	// ========

		Vtx arrCube[24] = {};
		// 윗면
		arrCube[0].pos = vec3(-0.5f, 0.5f, 0.5f);
		arrCube[0].color = vec4(1.f, 1.f, 1.f, 1.f);
		arrCube[0].uv = vec2(0.f, 0.f);
		arrCube[0].normal = vec3(0.f, 1.f, 0.f);

		arrCube[1].pos = vec3(0.5f, 0.5f, 0.5f);
		arrCube[1].color = vec4(1.f, 1.f, 1.f, 1.f);
		arrCube[1].uv = vec2(0.f, 0.f);
		arrCube[1].normal = vec3(0.f, 1.f, 0.f);

		arrCube[2].pos = vec3(0.5f, 0.5f, -0.5f);
		arrCube[2].color = vec4(1.f, 1.f, 1.f, 1.f);
		arrCube[2].uv = vec2(0.f, 0.f);
		arrCube[2].normal = vec3(0.f, 1.f, 0.f);

		arrCube[3].pos = vec3(-0.5f, 0.5f, -0.5f);
		arrCube[3].color = vec4(1.f, 1.f, 1.f, 1.f);
		arrCube[3].uv = vec2(0.f, 0.f);
		arrCube[3].normal = vec3(0.f, 1.f, 0.f);


		// 아랫 면	
		arrCube[4].pos = vec3(-0.5f, -0.5f, -0.5f);
		arrCube[4].color = vec4(1.f, 0.f, 0.f, 1.f);
		arrCube[4].uv = vec2(0.f, 0.f);
		arrCube[4].normal = vec3(0.f, -1.f, 0.f);

		arrCube[5].pos = vec3(0.5f, -0.5f, -0.5f);
		arrCube[5].color = vec4(1.f, 0.f, 0.f, 1.f);
		arrCube[5].uv = vec2(0.f, 0.f);
		arrCube[5].normal = vec3(0.f, -1.f, 0.f);

		arrCube[6].pos = vec3(0.5f, -0.5f, 0.5f);
		arrCube[6].color = vec4(1.f, 0.f, 0.f, 1.f);
		arrCube[6].uv = vec2(0.f, 0.f);
		arrCube[6].normal = vec3(0.f, -1.f, 0.f);

		arrCube[7].pos = vec3(-0.5f, -0.5f, 0.5f);
		arrCube[7].color = vec4(1.f, 0.f, 0.f, 1.f);
		arrCube[7].uv = vec2(0.f, 0.f);
		arrCube[7].normal = vec3(0.f, -1.f, 0.f);

		// 왼쪽 면
		arrCube[8].pos = vec3(-0.5f, 0.5f, 0.5f);
		arrCube[8].color = vec4(0.f, 1.f, 0.f, 1.f);
		arrCube[8].uv = vec2(0.f, 0.f);
		arrCube[8].normal = vec3(-1.f, 0.f, 0.f);

		arrCube[9].pos = vec3(-0.5f, 0.5f, -0.5f);
		arrCube[9].color = vec4(0.f, 1.f, 0.f, 1.f);
		arrCube[9].uv = vec2(0.f, 0.f);
		arrCube[9].normal = vec3(-1.f, 0.f, 0.f);

		arrCube[10].pos = vec3(-0.5f, -0.5f, -0.5f);
		arrCube[10].color = vec4(0.f, 1.f, 0.f, 1.f);
		arrCube[10].uv = vec2(0.f, 0.f);
		arrCube[10].normal = vec3(-1.f, 0.f, 0.f);

		arrCube[11].pos = vec3(-0.5f, -0.5f, 0.5f);
		arrCube[11].color = vec4(0.f, 1.f, 0.f, 1.f);
		arrCube[11].uv = vec2(0.f, 0.f);
		arrCube[11].normal = vec3(-1.f, 0.f, 0.f);

		// 오른쪽 면
		arrCube[12].pos = vec3(0.5f, 0.5f, -0.5f);
		arrCube[12].color = vec4(0.f, 0.f, 1.f, 1.f);
		arrCube[12].uv = vec2(0.f, 0.f);
		arrCube[12].normal = vec3(1.f, 0.f, 0.f);

		arrCube[13].pos = vec3(0.5f, 0.5f, 0.5f);
		arrCube[13].color = vec4(0.f, 0.f, 1.f, 1.f);
		arrCube[13].uv = vec2(0.f, 0.f);
		arrCube[13].normal = vec3(1.f, 0.f, 0.f);

		arrCube[14].pos = vec3(0.5f, -0.5f, 0.5f);
		arrCube[14].color = vec4(0.f, 0.f, 1.f, 1.f);
		arrCube[14].uv = vec2(0.f, 0.f);
		arrCube[14].normal = vec3(1.f, 0.f, 0.f);

		arrCube[15].pos = vec3(0.5f, -0.5f, -0.5f);
		arrCube[15].color = vec4(0.f, 0.f, 1.f, 1.f);
		arrCube[15].uv = vec2(0.f, 0.f);
		arrCube[15].normal = vec3(1.f, 0.f, 0.f);

		// 뒷 면
		arrCube[16].pos = vec3(0.5f, 0.5f, 0.5f);
		arrCube[16].color = vec4(1.f, 1.f, 0.f, 1.f);
		arrCube[16].uv = vec2(0.f, 0.f);
		arrCube[16].normal = vec3(0.f, 0.f, 1.f);

		arrCube[17].pos = vec3(-0.5f, 0.5f, 0.5f);
		arrCube[17].color = vec4(1.f, 1.f, 0.f, 1.f);
		arrCube[17].uv = vec2(0.f, 0.f);
		arrCube[17].normal = vec3(0.f, 0.f, 1.f);

		arrCube[18].pos = vec3(-0.5f, -0.5f, 0.5f);
		arrCube[18].color = vec4(1.f, 1.f, 0.f, 1.f);
		arrCube[18].uv = vec2(0.f, 0.f);
		arrCube[18].normal = vec3(0.f, 0.f, 1.f);

		arrCube[19].pos = vec3(0.5f, -0.5f, 0.5f);
		arrCube[19].color = vec4(1.f, 1.f, 0.f, 1.f);
		arrCube[19].uv = vec2(0.f, 0.f);
		arrCube[19].normal = vec3(0.f, 0.f, 1.f);

		// 앞 면
		arrCube[20].pos = vec3(-0.5f, 0.5f, -0.5f);;
		arrCube[20].color = vec4(1.f, 0.f, 1.f, 1.f);
		arrCube[20].uv = vec2(0.f, 0.f);
		arrCube[20].normal = vec3(0.f, 0.f, -1.f);

		arrCube[21].pos = vec3(0.5f, 0.5f, -0.5f);
		arrCube[21].color = vec4(1.f, 0.f, 1.f, 1.f);
		arrCube[21].uv = vec2(0.f, 0.f);
		arrCube[21].normal = vec3(0.f, 0.f, -1.f);

		arrCube[22].pos = vec3(0.5f, -0.5f, -0.5f);
		arrCube[22].color = vec4(1.f, 0.f, 1.f, 1.f);
		arrCube[22].uv = vec2(0.f, 0.f);
		arrCube[22].normal = vec3(0.f, 0.f, -1.f);

		arrCube[23].pos = vec3(-0.5f, -0.5f, -0.5f);
		arrCube[23].color = vec4(1.f, 0.f, 1.f, 1.f);
		arrCube[23].uv = vec2(0.f, 0.f);
		arrCube[23].normal = vec3(0.f, 0.f, -1.f);

		// 인덱스
		for (int i = 0; i < 12; i += 2)
		{
			vecIdx.push_back(i * 2);
			vecIdx.push_back(i * 2 + 1);
			vecIdx.push_back(i * 2 + 2);

			vecIdx.push_back(i * 2);
			vecIdx.push_back(i * 2 + 2);
			vecIdx.push_back(i * 2 + 3);
		}

		pMesh = new Mesh(true);
		pMesh->Create(arrCube, 24, vecIdx.data(), (UINT)vecIdx.size());
		AddRes<Mesh>(L"CubeMesh", pMesh);
		vecIdx.clear();

		// ===========
		// Sphere Mesh
		// ===========
		fRadius = 0.5f;

		// Top
		v.pos = vec3(0.f, fRadius, 0.f);
		v.uv = vec2(0.5f, 0.f);
		v.color = vec4(1.f, 1.f, 1.f, 1.f);
		v.normal = v.pos;
		v.normal.Normalize();
		v.tangent = vec3(1.f, 0.f, 0.f);
		v.binormal = vec3(0.f, 0.f, -1.f);
		vecVtx.push_back(v);

		// Body
		UINT iStackCount = 40; // 가로 분할 개수
		UINT iSliceCount = 40; // 세로 분할 개수

		float fStackAngle = XM_PI / iStackCount;
		float fSliceAngle = XM_2PI / iSliceCount;

		float fUVXStep = 1.f / (float)iSliceCount;
		float fUVYStep = 1.f / (float)iStackCount;

		for (UINT i = 1; i < iStackCount; ++i)
		{
			float phi = i * fStackAngle;

			for (UINT j = 0; j <= iSliceCount; ++j)
			{
				float theta = j * fSliceAngle;

				v.pos = vec3(fRadius * sinf(i * fStackAngle) * cosf(j * fSliceAngle)
					, fRadius * cosf(i * fStackAngle)
					, fRadius * sinf(i * fStackAngle) * sinf(j * fSliceAngle));

				v.uv = vec2(fUVXStep * j, fUVYStep * i);
				v.color = vec4(1.f, 1.f, 1.f, 1.f);
				v.normal = v.pos;
				v.normal.Normalize();

				v.tangent.x = -fRadius * sinf(phi) * sinf(theta);
				v.tangent.y = 0.f;
				v.tangent.z = fRadius * sinf(phi) * cosf(theta);
				v.tangent.Normalize();

				v.normal.Cross(v.tangent, v.binormal);
				v.binormal.Normalize();

				vecVtx.push_back(v);
			}
		}

		// Bottom
		v.pos = vec3(0.f, -fRadius, 0.f);
		v.uv = vec2(0.5f, 1.f);
		v.color = vec4(1.f, 1.f, 1.f, 1.f);
		v.normal = v.pos;
		v.normal.Normalize();

		v.tangent = vec3(1.f, 0.f, 0.f);
		v.binormal = vec3(0.f, 0.f, -1.f);
		vecVtx.push_back(v);

		// 인덱스
		// 북극점
		for (UINT i = 0; i < iSliceCount; ++i)
		{
			vecIdx.push_back(0);
			vecIdx.push_back(i + 2);
			vecIdx.push_back(i + 1);
		}

		// 몸통
		for (UINT i = 0; i < iStackCount - 2; ++i)
		{
			for (UINT j = 0; j < iSliceCount; ++j)
			{
				// + 
				// | \
				// +--+
				vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
				vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
				vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j)+1);

				// +--+
				//  \ |
				//    +
				vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
				vecIdx.push_back((iSliceCount + 1) * (i)+(j + 1) + 1);
				vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
			}
		}

		// 남극점
		UINT iBottomIdx = (UINT)vecVtx.size() - 1;
		for (UINT i = 0; i < iSliceCount; ++i)
		{
			vecIdx.push_back(iBottomIdx);
			vecIdx.push_back(iBottomIdx - (i + 2));
			vecIdx.push_back(iBottomIdx - (i + 1));
		}

		pMesh = new Mesh(true);
		pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
		AddRes<Mesh>(L"SphereMesh", pMesh);
		vecVtx.clear();
		vecIdx.clear();
	}

	void ResourceMgr::CreateDefaultGraphicsShader()
	{
		Ptr<GraphicsShader> shader = nullptr;

		shader = new GraphicsShader;
		shader->SetKey(L"Default2D_Shader");
		shader->CreateVertexShader(L"shader\\Default2D_VS.hlsl", "main");
		shader->CreatePixelShader(L"shader\\Default2D_PS.hlsl", "main");
		
		shader->SetRSType(RS_TYPE::CULL_NONE);
		shader->SetDSType(DS_TYPE::LESS);
		shader->SetBSType(BS_TYPE::MASK);
		
		shader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);
		shader->AddTexParam(TEX_0, "Output Texture");
		
		AddRes(shader->GetKey(), shader);

		//*******************************************************
		//*******************************************************

		shader = new GraphicsShader;
		shader->SetKey(L"Default3D_Shader");
		shader->CreateVertexShader(L"shader\\Default3D_VS.hlsl", "main");
		shader->CreatePixelShader(L"shader\\Default3D_PS.hlsl", "main");

		shader->SetRSType(RS_TYPE::CULL_NONE);
		shader->SetDSType(DS_TYPE::LESS);
		shader->SetBSType(BS_TYPE::MASK);

		shader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);
		shader->AddTexParam(TEX_0, "Output Texture");

		AddRes(shader->GetKey(), shader);

		//*******************************************************
		//*******************************************************

		shader = new GraphicsShader;
		shader->SetKey(L"DeferredDefault3D_Shader");
		shader->CreateVertexShader(L"shader\\Default3D_VS.hlsl", "main");
		shader->CreatePixelShader(L"shader\\DeferredDefault3D_PS.hlsl", "main");

		shader->SetRSType(RS_TYPE::CULL_BACK);
		shader->SetDSType(DS_TYPE::LESS_EQUAL);
		shader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);

		shader->AddTexParam(TEX_0, "Output Texture");

		AddRes(shader->GetKey(), shader);

		//*******************************************************
		//*******************************************************

		shader = new GraphicsShader;
		shader->SetKey(L"Std3DShader_Wire");
		shader->CreateVertexShader(L"shader\\Default3D_VS.hlsl", "main");
		shader->CreatePixelShader(L"shader\\Default3D_PS.hlsl", "main");

		shader->SetRSType(RS_TYPE::WIRE_FRAME);
		shader->SetDSType(DS_TYPE::LESS);
		shader->SetBSType(BS_TYPE::MASK);

		shader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);
		shader->AddTexParam(TEX_0, "Output Texture");

		AddRes(shader->GetKey(), shader);

		//*******************************************************
		//*******************************************************

		shader = new GraphicsShader;
		shader->SetKey(L"Std3DShader_Cull_Back");
		shader->CreateVertexShader(L"shader\\Default3D_VS.hlsl", "main");
		shader->CreatePixelShader(L"shader\\Default3D_PS.hlsl", "main");

		shader->SetRSType(RS_TYPE::CULL_BACK);
		shader->SetDSType(DS_TYPE::LESS);
		shader->SetBSType(BS_TYPE::MASK);

		shader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);
		shader->AddTexParam(TEX_0, "Output Texture");

		AddRes(shader->GetKey(), shader);

		//*******************************************************
		//*******************************************************

		shader = new GraphicsShader;
		shader->SetKey(L"Std3DShader_TrasnformArrow");
		shader->CreateVertexShader(L"shader\\Default3D_VS.hlsl", "main");
		shader->CreatePixelShader(L"shader\\Default3D_PS.hlsl", "main");

		shader->SetRSType(RS_TYPE::CULL_BACK);
		shader->SetDSType(DS_TYPE::LESS);
		shader->SetBSType(BS_TYPE::MASK);

		shader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);
		//pShader->AddTexParam(TEX_0, "Output Texture");

		AddRes(shader->GetKey(), shader);

		//*******************************************************
		//*******************************************************

		shader = new GraphicsShader;
		shader->SetKey(L"SkyBoxShader");
		shader->CreateVertexShader(L"shader\\SkyBox_VS.hlsl", "main");
		shader->CreatePixelShader(L"shader\\SkyBox_PS.hlsl", "main");

		shader->SetRSType(RS_TYPE::CULL_FRONT);
		shader->SetDSType(DS_TYPE::LESS_EQUAL);
		shader->SetBSType(BS_TYPE::DEFAULT);
		shader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

		shader->AddScalarParam(INT_0, "SkyBox Type");
		shader->AddTexParam(TEX_0, "Sphere Texture");
		shader->AddTexParam(TEX_CUBE_0, "Cube Texture");
		AddRes(shader->GetKey(), shader);
	}

	void ResourceMgr::CreateDefaultComputeShader()
	{
		Ptr<ComputeShader> pCS = nullptr;

		// Texture 색상 변경 쉐이더
		//pCS = new SetColorShader(32, 32, 1);
		//pCS->SetKey(L"SetColorCS");
		//pCS->CreateComputeShader(L"shader\\setcolor.fx", "CS_SetColor");
		//AddRes(pCS->GetKey(), pCS);
		//
		//// Particle Update 쉐이더
		//pCS = new ParticleShader(128, 1, 1);
		//pCS->SetKey(L"ParticleUpdateCS");
		//pCS->CreateComputeShader(L"shader\\particle_update.fx", "CS_ParticleUpdate");
		//AddRes(pCS->GetKey(), pCS);
	}

	void ResourceMgr::CreateDefaultMaterial()
	{
		Ptr<Material> material = nullptr;

		// Std2D Material
		material = new Material(true);
		material->SetShader(FindRes<GraphicsShader>(L"Default2D_Shader"));
		AddRes(L"Default2D_Mtrl", material);

		// Std2D Material
		material = new Material(true);
		material->SetShader(FindRes<GraphicsShader>(L"Default3D_Shader"));
		AddRes(L"Default3D_Mtrl", material);

		material = new Material(true);
		material->SetShader(FindRes<GraphicsShader>(L"DeferredDefault3D_Shader"));
		AddRes(L"DeferredDefault3D_Mtrl", material);

		// DebugShape Material
		material = new Material(true);
		material->SetShader(FindRes<GraphicsShader>(L"DebugShapeShader"));
		AddRes(L"DebugShapeMtrl", material);


		material = new Material(true);
		material->SetShader(FindRes<GraphicsShader>(L"Std3DShader_Wire"));
		//material->SetTexParam(TEX_0);
		AddRes(L"3DWireMtrl", material);

		material = new Material(true);
		material->SetShader(FindRes<GraphicsShader>(L"Std3DShader_Cull_Back"));
		AddRes(L"3DCullBackMtrl", material);

		material = new Material(true);
		material->SetShader(FindRes<GraphicsShader>(L"Std3DShader_TrasnformArrow"));
		AddRes(L"TransformArrowMtrl", material);

		material = new Material(true);
		material->SetShader(FindRes<GraphicsShader>(L"SkyBoxShader"));
		AddRes(L"SkyBoxMtrl", material);
	}
}