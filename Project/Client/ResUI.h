#pragma once
#include "UI.h"

#include <Engine\ptr.h>
#include <Engine\Resource.h>

namespace ff7r
{
    class ResourceUI : public UI
    {
    public:
        ResourceUI(RES_TYPE _type);
        ~ResourceUI();

        virtual int RenderUpdate() override;

        void SetTargetRes(Ptr<Resource> _Res) { target_res = _Res; }
        Ptr<Resource> GetTargetRes() { return target_res; }

    private:
        Ptr<Resource>   target_res;
        const RES_TYPE  res_type;

    };

}