#pragma once
#include "UI.h"

namespace ff7r
{
    class MenuUI :
        public UI
    {
    private:


    public:
        virtual void FinalTick() override;
        virtual int RenderUpdate() override;



    public:
        MenuUI();
        ~MenuUI();
    };

}