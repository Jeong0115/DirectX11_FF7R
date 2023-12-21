#include "pch.h"
#include "TestLevel.h"

#include <Engine/LevelMgr.h>
#include <Engine/Level.h>
#include <Engine/Layer.h>
#include <Engine/GameObject.h>
#include <Engine/GameEngine.h>
#include <Engine/components.h>

#include <Engine/ResourceMgr.h>
#include <Engine/CollisionMgr.h>
				
#include <Script/PlayerScript.h>
#include <Script/MonsterScript.h>

#include "LevelSaveLoad.h"

#include <Engine/Prefab.h>
namespace ff7r
{
	void CreateTestLevel()
	{
		//return;

		//Ptr<CSound> pSound = ResourceMgr::GetInst()->FindRes<CSound>(L"sound\\BGM_Stage1.wav");
		//pSound->Play(1, 0.5f, false);

		Level* pCurLevel = LevelMgr::GetInst()->GetCurLevel();
		pCurLevel->ChangeState(LEVEL_STATE::STOP);
		//pCurLevel->Set
		// Layer 이름설정
		pCurLevel->GetLayer(0)->SetName(L"Default");
		pCurLevel->GetLayer(1)->SetName(L"Tile");
		pCurLevel->GetLayer(2)->SetName(L"Player");
		pCurLevel->GetLayer(3)->SetName(L"Monster");
		pCurLevel->GetLayer(4)->SetName(L"PlayerProjectile");
		pCurLevel->GetLayer(5)->SetName(L"MonsterProjectile");
		pCurLevel->GetLayer(31)->SetName(L"UI");


		// Main Camera Object 생성
		GameObject* pMainCam = new GameObject;
		pMainCam->SetName(L"MainCamera");

		pMainCam->AddComponent(new Transform);
		pMainCam->AddComponent(new Camera);

		pMainCam->GetCamera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		pMainCam->GetCamera()->SetCameraIndex(0);		// MainCamera 로 설정
		pMainCam->GetCamera()->SetLayerMaskAll(true);	// 모든 레이어 체크
		pMainCam->GetCamera()->SetLayerMask(31, false);// UI Layer 는 렌더링하지 않는다.

		SpawnGameObject(pMainCam, vec3(0.f, 0.f, 0.f), 0);

		// UI cameara
		GameObject* pUICam = new GameObject;
		pUICam->SetName(L"UICamera");

		pUICam->AddComponent(new Transform);
		pUICam->AddComponent(new Camera);

		pUICam->GetCamera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		pUICam->GetCamera()->SetCameraIndex(1);		// Sub 카메라로 지정
		pUICam->GetCamera()->SetLayerMask(31, true);	// 31번 레이어만 체크

		SpawnGameObject(pUICam, vec3(0.f, 0.f, 0.f), 0);

		GameObject* ui_canvas = new GameObject;
		ui_canvas->SetName(L"Canvas");
		ui_canvas->AddComponent(new Transform);

		vec2 win_size = Engine::GetInst()->GetWindowResolution();
		ui_canvas->GetTransform()->SetRelativeScale(vec3(win_size.x, win_size.y, 1.0f));
		SpawnGameObject(ui_canvas, vec3(win_size.x / 2.f, win_size.y / 2.f, 0.f), L"UI");
	

		// 광원 추가
		GameObject* pLightObj = new GameObject;
		pLightObj->SetName(L"Point Light");

		pLightObj->AddComponent(new Transform);
		pLightObj->AddComponent(new Light3D);

		pLightObj->GetTransform()->SetRelativePos(vec3(0.f, 0.f, 0.f));
		//pLightObj->GetTransform()->SetRelativeRot(vec3(0.f, 0.f, XM_PI / 2.f));

		//pLightObj->GetTransform()->SetRelativeRot(vec3(XM_PI / 4.f, XM_PI / 4.f, 0.f));
		//pLightObj->GetLight3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
		//pLightObj->GetLight3D()->SetLightDiffuse(vec3(1.f, 1.f, 1.f));
		//pLightObj->GetLight3D()->SetLightAmbient(vec3(0.15f, 0.15f, 0.15f));

		//pLightObj->GetLight3D()->SetLightType(LIGHT_TYPE::POINT);
		//pLightObj->GetLight3D()->SetLightDiffuse(vec3(1.f, 0.2f, 0.2f));
		//pLightObj->GetLight3D()->SetLightAmbient(vec3(0.15f, 0.15f, 0.15f));
		//pLightObj->GetLight3D()->SetRadius(1000.f);

		pLightObj->GetTransform()->SetRelativeRot(vec3(XM_PI / 6.f, XM_PI / 4.f, 0.f));
		pLightObj->GetLight3D()->SetLightType(LIGHT_TYPE::SPOT);
		pLightObj->GetLight3D()->SetLightDiffuse(vec3(1.f, 1.f, 1.f));
		pLightObj->GetLight3D()->SetLightAmbient(vec3(0.15f, 0.15f, 0.15f));
		pLightObj->GetLight3D()->SetLightDirection(vec3(1.0f, 1.0f, 1.0f));
		pLightObj->GetLight3D()->SetRadius(2500.f);
		pLightObj->GetLight3D()->SetAngle(XM_PI / 6.f);

		SpawnGameObject(pLightObj, vec3(-700.f, -400.f, 0.f), 0);

		// 오브젝트 생성
		//GameObject* pParent = new GameObject;
		//pParent->SetName(L"Player");
		//pParent->AddComponent(new Transform);
		//pParent->AddComponent(new MeshRender);
		//pParent->AddComponent(new PlayerScript);
		//pParent->GetTransform()->SetRelativeScale(vec3(700.f, 700.f, 700.f));
		//Ptr<Texture> pAnimAtlas = ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\PC0000_00_BodyA_C.png");
		//
		//pParent->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"PC0000_000"));
		//pParent->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"Default3D_Mtrl"));
		//pParent->GetMeshRender()->GetMaterial()->SetTexParam(TEX_0, pAnimAtlas);

		//SpawnGameObject(pParent, vec3(0.f, -400.f, 700.f), L"Player");
		{
			GameObject* pParen1t = new GameObject;
			pParen1t->SetName(L"Cube");
			pParen1t->AddComponent(new Transform);
			pParen1t->AddComponent(new MeshRender);

			pParen1t->GetTransform()->SetRelativeScale(vec3(300.f, 300.f, 300.f));
			pParen1t->GetTransform()->SetRelativePos(0.0f, 0.f, 0.f);

			Ptr<Texture> pAnimAtlas = ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\T_Building_01_0Highway_C.png");

			pParen1t->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"CubeMesh"));
			pParen1t->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"Default3D_Mtrl"));
			//pParen1t->GetMeshRender()->GetMaterial()->SetTexParam(TEX_0, pAnimAtlas);

			SpawnGameObject(pParen1t, vec3(500.f, 0.f, 500.f), L"Player");
		}

		{
			GameObject* pParent = new GameObject;
			pParent->SetName(L"Floor");
			pParent->AddComponent(new Transform);
			pParent->AddComponent(new MeshRender);

			pParent->GetTransform()->SetRelativeScale(vec3(2000.f, 2000.f, 2000.f));
			pParent->GetTransform()->SetRelativeRot(vec3(XM_PI / 2.f, 0.f, 0.f));
					
			pParent->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh"));
			pParent->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"Default3D_Mtrl"));

			SpawnGameObject(pParent, vec3(0.f, -500.f, 0.f), L"Default");
		}

		//GameObject* pSkyBox = new GameObject;
		//pSkyBox->SetName(L"SkyBox");
		//
		//pSkyBox->AddComponent(new Transform);
		//pSkyBox->AddComponent(new SkyBox);
		//
		//pSkyBox->GetTransform()->SetRelativeScale(vec3(100.f, 100.f, 100));
		//pSkyBox->GetSkyBox()->SetSkyBoxTexture(ResourceMgr::GetInst()->FindRes<Texture>(L"texture\\skybox\\SkyDawn.dds"));
		//
		//SpawnGameObject(pSkyBox, vec3(0.f, 0.f, 0.f), 0);

		// 충돌 시킬 레이어 짝 지정
		CollisionMgr::GetInst()->LayerCheck(L"Player", L"Monster");
	}
}