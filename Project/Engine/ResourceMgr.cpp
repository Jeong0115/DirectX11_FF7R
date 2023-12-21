#include "pch.h"

#include "PathMgr.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "SetColorShader.h"
#include "ParticleShader.h"

#include "ResourceMgr.h"
namespace ff7r
{
	ResourceMgr::ResourceMgr()
		: is_change(false)
	{
	}

	ResourceMgr::~ResourceMgr()
	{
	}

	void ResourceMgr::Init()
	{
		InitSound();

		CreateDefaultMesh();
		CreateDefaultGraphicsShader();
		CreateDefaultComputeShader();
		CreateDefaultMaterial();
	}

	void ResourceMgr::Tick()
	{
		is_change = false;
	}

	Ptr<Texture> ResourceMgr::CreateTexture(const wstring& _strKey, UINT _Width, UINT _Height
		, DXGI_FORMAT _pixelformat, UINT _BindFlag, D3D11_USAGE _Usage)
	{
		Ptr<Texture> pTex = FindRes<Texture>(_strKey);

		assert(nullptr == pTex);

		pTex = new Texture(true);
		if (FAILED(pTex->Create(_Width, _Height, _pixelformat, _BindFlag, _Usage)))
		{
			assert(nullptr);
		}

		AddRes<Texture>(_strKey, pTex);

		return pTex;
	}

	Ptr<Texture> ResourceMgr::CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _Tex2D)
	{
		Ptr<Texture> pTex = FindRes<Texture>(_strKey);

		assert(nullptr == pTex);

		pTex = new Texture(true);
		if (FAILED(pTex->Create(_Tex2D)))
		{
			assert(nullptr);
		}

		AddRes<Texture>(_strKey, pTex);

		return pTex;
	}


	void ResourceMgr::DeleteRes(RES_TYPE _type, const wstring& _strKey)
	{
		map<wstring, Ptr<Resource>>::iterator iter = resources[(UINT)_type].find(_strKey);

		assert(!(iter == resources[(UINT)_type].end()));

		resources[(UINT)_type].erase(iter);

		is_change = true;
	}


	void ResourceMgr::ChangeResKey(const wstring _oldkey, const wstring& _newkey, Resource* _res)
	{
		RES_TYPE type = _res->GetType();

		map<wstring, Ptr<Resource>>::iterator iter = resources[(UINT)type].find(_oldkey);
		if (iter == resources[(UINT)type].end())
			return;

		DeleteRes(type, _oldkey);
		AddRes(_newkey, _res);
		_res->SetRelativePath(_newkey);

		wstring content_path = PathMgr::GetInst()->GetContentPath();

		if (std::rename(WStrToStr(content_path + _oldkey).c_str(), WStrToStr(content_path + _newkey).c_str()) != 0)
		{
			assert(false);
		}
		_res->Save(_newkey);
		// 머로 해야되지?
		is_change = true;
	}

	void ResourceMgr::AddRes(const wstring& _key, Resource* _res)
	{
		//assert(!FindRes<T>(_key).Get());

		RES_TYPE type = _res->GetType();
		resources[(UINT)type].insert(make_pair(_key, _res));
		_res->SetKey(_key);

		is_change = true;
	}
}