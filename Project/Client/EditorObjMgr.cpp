#include "pch.h"
#include "EditorObjMgr.h"

#include "GameObjectEx.h"
#include "TransformTool.h"
#include <Engine\components.h>

#include <Engine\ResourceMgr.h>
#include <Engine\RenderMgr.h>
#include <Engine\TimeMgr.h>
#include <Engine\KeyMgr.h>

#include <Script\CameraMoveScript.h>
namespace ff7r
{
	EditorObjMgr::EditorObjMgr()
		: m_DebugShape{}
	{

	}

	EditorObjMgr::~EditorObjMgr()
	{
		Safe_Del_Vec(m_vecEditorObj);
		Safe_Del_Array(m_DebugShape);

		delete transform_tool;
	}

	void EditorObjMgr::Init()
	{
		transform_tool = new TransformTool;
		transform_tool->AddComponent(new Transform);
		transform_tool->AddComponent(new MeshRender);
		transform_tool->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"scene2"));
		transform_tool->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"TransformArrowMtrl"));
		transform_tool->GetTransform()->SetAbsolute(true);
		// 디버그 쉐이프 생성
		m_DebugShape[(UINT)SHAPE_TYPE::RECT] = new GameObjectEx;
		m_DebugShape[(UINT)SHAPE_TYPE::RECT]->AddComponent(new Transform);
		m_DebugShape[(UINT)SHAPE_TYPE::RECT]->AddComponent(new MeshRender);
		m_DebugShape[(UINT)SHAPE_TYPE::RECT]->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"RectMesh_Debug"));
		m_DebugShape[(UINT)SHAPE_TYPE::RECT]->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"DebugShapeMtrl"));
		
		m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE] = new GameObjectEx;
		m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE]->AddComponent(new Transform);
		m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE]->AddComponent(new MeshRender);
		m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE]->GetMeshRender()->SetMesh(ResourceMgr::GetInst()->FindRes<Mesh>(L"CircleMesh_Debug"));
		m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE]->GetMeshRender()->SetMaterial(ResourceMgr::GetInst()->FindRes<Material>(L"DebugShapeMtrl"));

		// EditorObject 생성
		GameObjectEx* pEditorCamObj = new GameObjectEx;
		pEditorCamObj->AddComponent(new Transform);
		pEditorCamObj->AddComponent(new Camera);
		pEditorCamObj->AddComponent(new CameraMoveScript);

		pEditorCamObj->GetCamera()->SetLayerMaskAll(true);
		//pEditorCamObj->GetCamera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		pEditorCamObj->GetCamera()->SetProjType(PROJ_TYPE::PERSPECTIVE);


		m_vecEditorObj.push_back(pEditorCamObj);
		RenderMgr::GetInst()->RegisterEditorCamera(pEditorCamObj->GetCamera());
	}

	void EditorObjMgr::Progress()
	{
		// DebugShape 정보 가져오기
		vector<DebugShapeInfo>& vecInfo = RenderMgr::GetInst()->GetDebugShapeInfo();
		m_DebugShapeInfo.insert(m_DebugShapeInfo.end(), vecInfo.begin(), vecInfo.end());
		vecInfo.clear();


		Tick();

		Render();
	}

	void EditorObjMgr::SetTransformTarget(GameObject* _object)
	{
		transform_tool->SetTarget(_object);
	}


	void EditorObjMgr::Tick()
	{
		for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
		{
			m_vecEditorObj[i]->Tick();
		}

		for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
		{
			m_vecEditorObj[i]->FinalTick();
		}
		transform_tool->Tick();
		transform_tool->FinalTick();
	}

	void EditorObjMgr::Render()
	{
		for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
		{
			m_vecEditorObj[i]->Render();
		}

		transform_tool->Render();

		// DebugShape Render
		GameObjectEx* pShapeObj = nullptr;

		return;
		vector<DebugShapeInfo>::iterator iter = m_DebugShapeInfo.begin();
		for (; iter != m_DebugShapeInfo.end();)
		{
			switch (iter->type)
			{
			case SHAPE_TYPE::RECT:
				pShapeObj = m_DebugShape[(UINT)SHAPE_TYPE::RECT];
				break;
			case SHAPE_TYPE::CIRCLE:
				pShapeObj = m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE];
				break;
			case SHAPE_TYPE::CUBE:
				break;
			case SHAPE_TYPE::SPHERE:
				break;
			}

			if (iter->mat_world != XMMatrixIdentity())
			{
				pShapeObj->GetTransform()->SetWorldMat(iter->mat_world);
			}
			else
			{
				pShapeObj->GetTransform()->SetRelativePos(iter->world_pos);
				pShapeObj->GetTransform()->SetRelativeScale(iter->world_scale);
				pShapeObj->GetTransform()->SetRelativeRot(iter->world_rotation);
				pShapeObj->FinalTick();
			}

			pShapeObj->GetMeshRender()->GetMaterial()->SetScalarParam(VEC4_0, &iter->color);
			pShapeObj->Render();

			iter->cur_time += DT;
			if (iter->max_time < iter->cur_time)
			{
				iter = m_DebugShapeInfo.erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}
}