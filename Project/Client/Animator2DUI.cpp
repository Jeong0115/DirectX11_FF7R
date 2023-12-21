#include "pch.h"
#include "Animator2DUI.h"

namespace ff7r
{
    Animator2DUI::Animator2DUI()
        : ComponentUI("##Animator2D", COMPONENT_TYPE::ANIMATOR2D)
    {
        SetName("Animator2D");
    }

    Animator2DUI::~Animator2DUI()
    {
    }


    int Animator2DUI::RenderUpdate()
    {
        if (FALSE == ComponentUI::RenderUpdate())
            return FALSE;




        return TRUE;
    }
}