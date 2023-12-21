#pragma once

#include "Entity.h"

namespace ff7r
{
    class ConstBuffer : public Entity
    {
    public:
        ConstBuffer(UINT _register_num);
        ~ConstBuffer();

        CLONE_DISABLE(ConstBuffer);

        void Create(UINT _element_size, UINT _element_count);
        void SetData(void* _src, UINT _size = 0);
        void UpdateData();
        void UpdateData_CS();

    private:
        ComPtr<ID3D11Buffer>    const_buff;
        D3D11_BUFFER_DESC       desc;
        const UINT              register_number;

        UINT                    element_size;
        UINT                    element_count;
    };
}
