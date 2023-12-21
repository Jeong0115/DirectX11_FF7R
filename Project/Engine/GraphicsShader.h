#pragma once

#include "Shader.h"

namespace ff7r
{
    struct ScalarParam
    {
        SCALAR_PARAM    param;
        string          desc;
    };

    struct TexParam
    {
        TEX_PARAM       param;
        string          desc;
    };

    class GraphicsShader : public Shader
    {
    public:
        GraphicsShader();
        ~GraphicsShader();

        void CreateVertexShader(const wstring& _file_name, const string& _func_name);
        void CreateGeometryShader(const wstring& _file_name, const string& _func_name);
        void CreatePixelShader(const wstring& _file_name, const string& _func_name);
        void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _topology) { topology = _topology; }
        void SetRSType(RS_TYPE _type) { rasterizer_type = _type; }
        void SetDSType(DS_TYPE _type) { depthstencil_type = _type; }
        void SetBSType(BS_TYPE _type) { blend_type = _type; }
        void SetDomain(SHADER_DOMAIN _domain) { domain = _domain; }

        SHADER_DOMAIN GetDomain() { return domain; }
        virtual void UpdateData() override;

        void AddScalarParam(SCALAR_PARAM _param, const string& _desc) { scalar_param.push_back(ScalarParam{ _param, _desc }); }
        void AddTexParam(TEX_PARAM _param, const string& _desc) { texture_param.push_back(TexParam{ _param, _desc }); }

        const vector<ScalarParam>& GetScalarParam() { return scalar_param; }
        const vector<TexParam>& GetTexParam() { return texture_param; }

    private:
        ComPtr<ID3DBlob>                vertex_shader_blob;
        ComPtr<ID3DBlob>                hull_shader_blob;
        ComPtr<ID3DBlob>                domain_shader_blob;
        ComPtr<ID3DBlob>                geomentry_shader_blob;
        ComPtr<ID3DBlob>                pixel_shader_blob;

        ComPtr<ID3D11VertexShader>      vertex_shader;
        ComPtr<ID3D11HullShader>        hull_shader;
        ComPtr<ID3D11DomainShader>      domain_shader;
        ComPtr<ID3D11GeometryShader>    geomentry_shader;
        ComPtr<ID3D11PixelShader>       pixel_shader;

        ComPtr<ID3D11InputLayout>	    input_layout;
        D3D11_PRIMITIVE_TOPOLOGY        topology;

        RS_TYPE                         rasterizer_type;
        DS_TYPE                         depthstencil_type;
        BS_TYPE                         blend_type;

        SHADER_DOMAIN                   domain;

        vector<ScalarParam>             scalar_param;
        vector<TexParam>                texture_param;
    };
}
