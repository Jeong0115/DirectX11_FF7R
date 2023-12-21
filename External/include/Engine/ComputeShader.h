#pragma once

#include "Shader.h"

namespace ff7r
{
    class ComputeShader : public Shader
    {
    public:
        ComputeShader();
        ~ComputeShader();

        void CreateComputeShader(const wstring& _file_name, const string& _func_name);
        void Execute();

    private:
        CLONE_DISABLE(ComputeShader);

        virtual void UpdateData() = 0;
        virtual void Clear() = 0;

    protected:
        MtrlConst                       mtrl_const_data;

        UINT                            group_x_cnt;
        UINT                            group_y_cnt;
        UINT                            group_z_cnt;

        UINT                            thread_x_per_group;
        UINT                            thread_y_per_group;
        UINT                            thread_z_per_group;

    private:    
        ComPtr<ID3D11ComputeShader>     compute_shader;
        ComPtr<ID3DBlob>                compute_shader_blob;
    };
}
