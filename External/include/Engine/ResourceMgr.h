#pragma once

#include "ptr.h"
#include "Mesh.h"
#include "Texture.h"
#include "GraphicsShader.h"
#include "ComputeShader.h"
#include "Material.h"
#include "Prefab.h"
#include "Sound.h"
#include "LevelRes.h"

#include "PathMgr.h"

namespace ff7r
{
    class ResourceMgr : public Singleton<ResourceMgr>
    {
    public:
        void Init();
        void Tick();

        void CreateNewResource(wstring _name, RES_TYPE _type);
        void CreatMeshFbx_Test(void* _VtxSysMem, UINT _iVtxCount, void* _IdxSysMem, UINT _IdxCount, wstring meshName);

        Ptr<Texture> CreateTexture(const wstring& _key, ComPtr<ID3D11Texture2D> _tex2d);
        Ptr<Texture> CreateTexture(const wstring& _key, UINT _width, UINT _height
            , DXGI_FORMAT _pixelformat, UINT _bindflag, D3D11_USAGE _usage = D3D11_USAGE::D3D11_USAGE_DEFAULT);

        const map<wstring, Ptr<Resource>>& GetResources(RES_TYPE _type) { return resources[(UINT)_type]; }
        bool IsResourceChanged() { return is_change; }

        void ChangeResKey(const wstring _oldkey, const wstring& _newkey, Resource* _res);
        void AddRes(const wstring& _key, Resource* _res);

        template<typename T> Ptr<T> FindRes(const wstring& _key);
        template<typename T> void   AddRes(const wstring& _key, Ptr<T>& _res);
        template<typename T> Ptr<T> Load(const wstring& _key, const wstring& _path);

    private:
        SINGLE(ResourceMgr)

        void InitSound();
        void CreateDefaultMesh();
        void CreateDefaultGraphicsShader();
        void CreateDefaultComputeShader();
        void CreateDefaultMaterial();
        void DeleteRes(RES_TYPE _type, const wstring& _strKey);

    private:
        map<wstring, Ptr<Resource>> resources[(UINT)RES_TYPE::END];
        bool                        is_change;

        friend class EventMgr;
    };

    template<typename T>
    RES_TYPE GetResType()
    {
        const type_info& level = typeid(LevelRes);
        const type_info& mesh = typeid(Mesh);
        //const type_info& meshdata = typeid(CMeshData);
        const type_info& material = typeid(Material);
        const type_info& texture = typeid(Texture);
        const type_info& sound = typeid(Sound);
        const type_info& prefab = typeid(Prefab);
        const type_info& gs = typeid(GraphicsShader);
        const type_info& cs = typeid(ComputeShader);

        if (typeid(T).hash_code() == level.hash_code())
            return RES_TYPE::LEVEL;
        if (typeid(T).hash_code() == mesh.hash_code())
            return RES_TYPE::MESH;
        if (typeid(T).hash_code() == gs.hash_code())
            return RES_TYPE::GRAPHICS_SHADER;
        if (typeid(T).hash_code() == cs.hash_code())
            return RES_TYPE::COMPUTE_SHADER;
        if (typeid(T).hash_code() == texture.hash_code())
            return RES_TYPE::TEXTURE;
        if (typeid(T).hash_code() == material.hash_code())
            return RES_TYPE::MATERIAL;
        if (typeid(T).hash_code() == prefab.hash_code())
            return RES_TYPE::PREFAB;
        if (typeid(T).hash_code() == sound.hash_code())
            return RES_TYPE::SOUND;

        return RES_TYPE::END;
    }


    template<typename T>
    inline Ptr<T> ResourceMgr::FindRes(const wstring& _key)
    {
        RES_TYPE type = GetResType<T>();

        map<wstring, Ptr<Resource>>::iterator iter = resources[(UINT)type].find(_key);
        if (iter == resources[(UINT)type].end())
            return nullptr;

        return (T*)iter->second.Get();
    }




    template<typename T>
    inline void ResourceMgr::AddRes(const wstring& _key, Ptr<T>& _res)
    {
        // 중복키로 리소스 추가하려는 경우
        assert(!FindRes<T>(_key).Get());

        RES_TYPE type = GetResType<T>();
        resources[(UINT)type].insert(make_pair(_key, _res.Get()));
        _res->SetKey(_key);
        _res->SetName(_key);

        is_change = true;
    }


    template<typename T>
    inline Ptr<T> ResourceMgr::Load(const wstring& _key, const wstring& _path)
    {
        Ptr<Resource> res = FindRes<T>(_key).Get();

        // 이미 해당 키로 리소스가 있다면, 반환
        if (nullptr != res)
            return (T*)res.Get();

        res = new T;
        res->SetKey(_key);
        res->SetRelativePath(_path);

        wstring path = PathMgr::GetInst()->GetContentPath();
        path += _path;

        if (FAILED(res->Load(path)))
        {
            assert(nullptr);
        }

        RES_TYPE type = GetResType<T>();
        resources[(UINT)type].insert(make_pair(_key, res));
        is_change = true;

        return (T*)res.Get();
    }
}
