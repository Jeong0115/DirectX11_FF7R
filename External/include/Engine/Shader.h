#pragma once

#include "Resource.h"

namespace ff7r
{
    class Shader : public Resource
    {
    public:
        Shader(RES_TYPE _type);
        ~Shader();

        virtual int Save(const wstring& _strRelativePath) { return S_OK; }

    private:
        virtual int Load(const wstring& _strFilePath) { return S_OK; }

    protected:
        ComPtr<ID3DBlob> error_blob;
    };
}

