#include "pch.h"
#include "Camera.h"


#include "Device.h"
#include "RenderMgr.h"
#include "Transform.h"

#include "LevelMgr.h"
#include "Level.h"
#include "Layer.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "RenderMgr.h"
#include "Material.h"
#include "MRT.h"
#include "GraphicsShader.h"

namespace ff7r
{
	Camera::Camera()
		: Component(COMPONENT_TYPE::CAMERA)
		, aspect_ratio(1.f)
		, scale(1.f)
		, proj_type(PROJ_TYPE::ORTHOGRAPHIC)
		, layer_mask(0)
		, camera_idx(-1)
		, mat_view{}
		, mat_proj{}
		, opaque_obj{}
		, mask_obj{}
		, transparent_obj{}
		, ui_obj{}
		, post_obj{}
	{
		SetName(L"Camera");

		vec2 vRenderResol = Device::GetInst()->GetRenderResolution();
		aspect_ratio = vRenderResol.x / vRenderResol.y;
	}

	Camera::Camera(const Camera& _other)
		: Component(_other)
		, aspect_ratio(_other.aspect_ratio)
		, scale(_other.scale)
		, proj_type(_other.proj_type)
		, layer_mask(_other.layer_mask)
		, camera_idx(-1)
	{
	}

	Camera::~Camera()
	{
	}

	void Camera::Begin()
	{
		if (-1 != camera_idx)
		{
			RenderMgr::GetInst()->RegisterCamera(this, camera_idx);
		}
	}

	void Camera::FinalTick()
	{
		CalcViewMat();
		CalcProjMat();
	}

	void Camera::CalcViewMat()
	{
		// ==============
		// View 행렬 계산
		// ==============
		mat_view = XMMatrixIdentity();

		// 카메라 좌표를 원점으로 이동
		vec3 camera_pos = GetTransform()->GetRelativePos();
		Matrix mat_transform = XMMatrixTranslation(-camera_pos.x, -camera_pos.y, -camera_pos.z);

		// 카메라가 바라보는 방향을 Z 축과 평행하게 만드는 회전 행렬을 적용
		Matrix mat_rotaion = XMMatrixIdentity();

		vec3 vR = GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);
		vec3 vU = GetTransform()->GetWorldDir(DIR_TYPE::UP);
		vec3 vF = GetTransform()->GetWorldDir(DIR_TYPE::FRONT);

		mat_rotaion._11 = vR.x;	mat_rotaion._12 = vU.x;	mat_rotaion._13 = vF.x;
		mat_rotaion._21 = vR.y;	mat_rotaion._22 = vU.y;	mat_rotaion._23 = vF.y;
		mat_rotaion._31 = vR.z;	mat_rotaion._32 = vU.z;	mat_rotaion._33 = vF.z;

		mat_view = mat_transform * mat_rotaion;
	}

	void Camera::CalcProjMat()
	{
		// =============
		// 투영 행렬 계산
		// =============
		mat_proj = XMMatrixIdentity();

		if (PROJ_TYPE::ORTHOGRAPHIC == proj_type)
		{
			// 직교 투영
			vec2 resolution = Device::GetInst()->GetRenderResolution();
			mat_proj = XMMatrixOrthographicLH(resolution.x * (1.f / scale), resolution.y * (1.f / scale), 1.f, 10000.f);
		}
		else
		{
			// 원근 투영
			mat_proj = XMMatrixPerspectiveFovLH(XM_PI / 2.f, aspect_ratio, 1.f, 10000.f);
		}
	}

	void Camera::SetLayerMask(int _layer, bool _visible)
	{
		if (_visible)
		{
			layer_mask |= 1 << _layer;
		}
		else
		{
			layer_mask &= ~(1 << _layer);
		}
	}

	void Camera::SetLayerMaskAll(bool _visible)
	{
		if (_visible)
			layer_mask = 0xffffffff;
		else
			layer_mask = 0;
	}

	void Camera::SetCameraIndex(int _idx)
	{
		camera_idx = _idx;
		RenderMgr::GetInst()->RegisterCamera(this, camera_idx);
	}

	void Camera::SortObject()
	{
		// 이전 프레임 분류정보 제거
		Clear();

		// 현재 레벨 가져와서 분류
		Level* level = LevelMgr::GetInst()->GetCurLevel();

		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			// 레이어 마스크 확인
			if (layer_mask & (1 << i))
			{
				Layer* layer = level->GetLayer(i);
				const vector<GameObject*>& objects = layer->GetObjects();

				for (size_t j = 0; j < objects.size(); ++j)
				{
					RenderComponent* render_component = objects[j]->GetRenderComponent();

					// 렌더링 기능이 없는 오브젝트는 제외
					if (nullptr == render_component
						|| nullptr == render_component->GetMaterial()
						|| nullptr == render_component->GetMaterial()->GetShader())
						continue;

					// 쉐이더 도메인에 따른 분류
					SHADER_DOMAIN domain = render_component->GetMaterial()->GetShader()->GetDomain();
					switch (domain)
					{
					case SHADER_DOMAIN::DOMAIN_DEFERRED:	deferred_obj.push_back(objects[j]);		break;
					case SHADER_DOMAIN::DOMAIN_OPAQUE:		opaque_obj.push_back(objects[j]);		break;
					case SHADER_DOMAIN::DOMAIN_MASK:		mask_obj.push_back(objects[j]);			break;
					case SHADER_DOMAIN::DOMAIN_TRANSPARENT: transparent_obj.push_back(objects[j]);	break;
					case SHADER_DOMAIN::DOMAIN_POSTPROCESS: post_obj.push_back(objects[j]);			break;
					case SHADER_DOMAIN::DOMAIN_UI:			ui_obj.push_back(objects[j]);			break;
					}
				}
			}
		}
	}

	void Camera::Render()
	{
		// 행렬 업데이트
		Tansform_Matrix.mat_view = mat_view;
		Tansform_Matrix.mat_proj = mat_proj;

		RenderMgr::GetInst()->GetMRT(MRT_TYPE::DEFERRED)->OMSetRenderTargets(true);
		RenderDeferred();

		RenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->OMSetRenderTargets();

		RenderOpaque();
		RenderMask();
		RenderTransparent();
		RenderPostprocess();

		RenderUI();
	}


	void Camera::Clear()
	{
		opaque_obj.clear();
		mask_obj.clear();
		transparent_obj.clear();
		post_obj.clear();
		ui_obj.clear();
		deferred_obj.clear();
	}

	void Camera::RenderDeferred()
	{
		for (size_t i = 0; i < deferred_obj.size(); ++i)
		{
			deferred_obj[i]->Render();
		}
	}

	void Camera::RenderOpaque()
	{
		for (size_t i = 0; i < opaque_obj.size(); ++i)
		{
			opaque_obj[i]->Render();
		}
	}

	void Camera::RenderMask()
	{
		for (size_t i = 0; i < mask_obj.size(); ++i)
		{
			mask_obj[i]->Render();
		}
	}

	void Camera::RenderTransparent()
	{
		for (size_t i = 0; i < transparent_obj.size(); ++i)
		{
			transparent_obj[i]->Render();
		}
	}

	void Camera::RenderPostprocess()
	{
		for (size_t i = 0; i < post_obj.size(); ++i)
		{
			RenderMgr::GetInst()->CopyRenderTarget();
			post_obj[i]->Render();
		}
	}

	void Camera::RenderUI()
	{
		for (size_t i = 0; i < ui_obj.size(); ++i)
		{
			ui_obj[i]->Render();
		}
	}


	void Camera::SaveToLevelFile(FILE* _file)
	{
		fwrite(&aspect_ratio, sizeof(float), 1, _file);
		fwrite(&scale, sizeof(float), 1, _file);
		fwrite(&proj_type, sizeof(UINT), 1, _file);
		fwrite(&layer_mask, sizeof(UINT), 1, _file);
		fwrite(&camera_idx, sizeof(int), 1, _file);
	}

	void Camera::LoadFromLevelFile(FILE* _file)
	{
		fread(&aspect_ratio, sizeof(float), 1, _file);
		fread(&scale, sizeof(float), 1, _file);
		fread(&proj_type, sizeof(UINT), 1, _file);
		fread(&layer_mask, sizeof(UINT), 1, _file);
		fread(&camera_idx, sizeof(int), 1, _file);
	}
}