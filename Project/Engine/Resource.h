#pragma once

#include "Entity.h"

namespace ff7r
{
    class Resource : public Entity
    {
    public:
        Resource(RES_TYPE _type, bool _engine_resource = false);
        Resource(const Resource& _other);
        virtual ~Resource();

        virtual int         Save(const wstring&) = 0;
        virtual Resource*   Clone() { return nullptr; assert(nullptr); }
        const wstring&      GetKey() { return key; }
        const wstring&      GetRelativePath() { return relative_path; }
        RES_TYPE            GetType() { return type; }
        bool                IsEngineRes() { return engine_resource; }

    private:
        virtual void        UpdateData() = 0;
        virtual int         Load(const wstring& _path) = 0;

        void                SetKey(const wstring& _key) { key = _key; }
        void                SetRelativePath(const wstring& _path) { relative_path = _path; }
        void                AddRef() { ++ref_count; }
        void                Release();

        const RES_TYPE      type;
        int                 ref_count;

        wstring             key;
        wstring             relative_path;

        bool                engine_resource;

        friend class ResourceMgr;
        template<typename T>
        friend class Ptr;
    };
}

