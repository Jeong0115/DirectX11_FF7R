#pragma once

#include "Entity.h"

namespace ff7r
{
    class StructuredBuffer : public Entity
    {
    public:
        StructuredBuffer();
        ~StructuredBuffer();

        CLONE_DISABLE(StructuredBuffer);

        void Create(UINT _element_size, UINT _element_count, SB_TYPE _type, bool _use_sys_access, void* _sys_mem = nullptr);
        void SetData(void* _src, UINT _size = 0);
        void GetData(void* _dst);

        // PIPELINE_STAGE
        void UpdateData(UINT _register_num, UINT _stage);
        void UpdateData_CS(UINT _register_num, bool _is_shader_res);

        void Clear();
        void Clear_CS(bool _is_shader_res);

        UINT GetElementSize() { return element_size; }
        UINT GetElementCount() { return element_count; }
        UINT GetBufferSize() { return element_size * element_count; }



    private:
        ComPtr<ID3D11Buffer>                structed_buff;   // register binding
        ComPtr<ID3D11ShaderResourceView>    shader_res_view;
        ComPtr<ID3D11UnorderedAccessView>   unoredered_access_view;
        ComPtr<ID3D11Buffer>                read_buff;  // GPU -> Sys
        ComPtr<ID3D11Buffer>                write_buff; // Sys -> GPU
        D3D11_BUFFER_DESC                   desc;

        SB_TYPE                             type;
        UINT                                element_size;
        UINT                                element_count;
        UINT                                register_number;

        bool                                system_access;
    };
}
