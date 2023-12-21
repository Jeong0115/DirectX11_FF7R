#pragma once

#include "ptr.h"

#include "GraphicsShader.h"
#include "Resource.h"
#include "Texture.h"

namespace ff7r
{
    class Material : public Resource
    {
    public:
        Material(bool _is_engine_res = false);
        ~Material();

        CLONE(Material);

        void SetScalarParam(SCALAR_PARAM _param, const void* _src);
        void SetTexParam(TEX_PARAM _param, const Ptr<Texture>& _tex);

        void GetScalarParam(SCALAR_PARAM _param, void* _data);
        Ptr<Texture> GetTexParam(TEX_PARAM _param) { return textures[(UINT)_param]; }

        void SetShader(Ptr<GraphicsShader> _shader) { shader = _shader; }
        Ptr<GraphicsShader> GetShader() { return shader; }

        virtual void UpdateData() override;
        virtual int Save(const wstring& _relative_path) override;

    private:
        virtual int Load(const wstring& _file_path) override;

        Ptr<GraphicsShader>     shader;
        MtrlConst               const_data;
        Ptr<Texture>            textures[TEX_END];

    };
}
