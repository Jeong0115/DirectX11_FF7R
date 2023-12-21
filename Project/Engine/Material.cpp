#include "pch.h"

#include "ConstBuffer.h"
#include "Device.h"
#include "ResourceMgr.h"
#include "PathMgr.h"

#include "Material.h"
namespace ff7r
{
	Material::Material(bool _is_engine_res)
		: Resource(RES_TYPE::MATERIAL, _is_engine_res)
		, shader{}
		, const_data{}
		, textures{}
	{
	}

	Material::~Material()
	{
	}

	void Material::UpdateData()
	{
		if (nullptr == shader)
		{
			return;
		}

		shader->UpdateData();


		// Texture Update
		for (UINT i = 0; i < TEX_END; ++i)
		{
			if (nullptr == textures[i])
			{
				const_data.array_tex[i] = 0;
				Texture::Clear(i);
				continue;
			}

			else
			{
				const_data.array_tex[i] = 1;
				textures[i]->UpdateData(i, PIPELINE_STAGE::PS_PIXEL);
			}
		}

		// Constant Update
		ConstBuffer* mtrl_buff = Device::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
		mtrl_buff->SetData(&const_data);
		mtrl_buff->UpdateData();
	}

	void Material::SetScalarParam(SCALAR_PARAM _param, const void* _src)
	{
		switch (_param)
		{
		case INT_0:
		case INT_1:
		case INT_2:
		case INT_3:
			const_data.array_int[_param] = *((int*)_src);
			break;
		case FLOAT_0:
		case FLOAT_1:
		case FLOAT_2:
		case FLOAT_3:
			const_data.array_float[_param - FLOAT_0] = *((float*)_src);
			break;

		case VEC2_0:
		case VEC2_1:
		case VEC2_2:
		case VEC2_3:
			const_data.array_vec2[_param - VEC2_0] = *((vec2*)_src);
			break;

		case VEC4_0:
		case VEC4_1:
		case VEC4_2:
		case VEC4_3:
			const_data.array_vec4[_param - VEC4_0] = *((vec4*)_src);
			break;

		case MAT_0:
		case MAT_1:
		case MAT_2:
		case MAT_3:
			const_data.array_mat[_param - MAT_0] = *((Matrix*)_src);
			break;
		}
	}

	void Material::SetTexParam(TEX_PARAM _param, const Ptr<Texture>& _tex)
	{
		textures[_param] = _tex;
	}

	void Material::GetScalarParam(SCALAR_PARAM _param, void* _data)
	{
		switch (_param)
		{
		case INT_0:
		case INT_1:
		case INT_2:
		case INT_3:
		{
			int idx = (UINT)_param - (UINT)INT_0;
			*((int*)_data) = const_data.array_int[idx];
		}
		break;
		case FLOAT_0:
		case FLOAT_1:
		case FLOAT_2:
		case FLOAT_3:
		{
			int idx = (UINT)_param - (UINT)FLOAT_0;
			*((float*)_data) = const_data.array_float[idx];
		}
		break;

		case VEC2_0:
		case VEC2_1:
		case VEC2_2:
		case VEC2_3:
		{
			int idx = (UINT)_param - (UINT)VEC2_0;
			*((vec2*)_data) = const_data.array_vec2[idx];
		}
		break;

		case VEC4_0:
		case VEC4_1:
		case VEC4_2:
		case VEC4_3:
		{
			int idx = (UINT)_param - (UINT)VEC4_0;
			*((vec4*)_data) = const_data.array_vec4[idx];
		}
		break;

		case MAT_0:
		case MAT_1:
		case MAT_2:
		case MAT_3:
		{
			int idx = (UINT)_param - (UINT)MAT_0;
			*((Matrix*)_data) = const_data.array_mat[idx];
		}
		break;
		}
	}

	// ================
	// File Save / Load
	// ================
	int Material::Save(const wstring& _relative_path)
	{
		if (IsEngineRes())
			return E_FAIL;

		wstring strFilePath = PathMgr::GetInst()->GetContentPath();
		strFilePath += _relative_path;

		FILE* pFile = nullptr;
		_wfopen_s(&pFile, strFilePath.c_str(), L"wb");

		// Entity
		SaveWString(GetName(), pFile);

		// Res
		SaveWString(GetKey(), pFile);

		// Shader

		SaveResRef(shader.Get(), pFile);


		// Constant
		fwrite(&const_data, sizeof(MtrlConst), 1, pFile);

		// Texture
		for (UINT i = 0; i < (UINT)TEX_PARAM::TEX_END; ++i)
		{
			SaveResRef(textures[i].Get(), pFile);
		}

		fclose(pFile);

		return S_OK;
	}


	int Material::Load(const wstring& _file_path)
	{
		FILE* pFile = nullptr;
		_wfopen_s(&pFile, _file_path.c_str(), L"rb");

		// Entity
		wstring strName;
		LoadWString(strName, pFile);
		SetName(strName);

		// Res
		wstring strKey;
		LoadWString(strKey, pFile);

		// Shader
		LoadResRef(shader, pFile);

		// Constant
		fread(&const_data, sizeof(MtrlConst), 1, pFile);

		// Texture
		for (UINT i = 0; i < (UINT)TEX_PARAM::TEX_END; ++i)
		{
			LoadResRef(textures[i], pFile);
		}

		fclose(pFile);

		return S_OK;
	}
}