#include "pch.h"
#include "Light2DUI.h"

namespace ff7r
{
    Light2DUI::Light2DUI()
        : ComponentUI("##Light2D", COMPONENT_TYPE::LIGHT2D)
    {
        SetName("Light2D");
    }

    Light2DUI::~Light2DUI()
    {
    }


    int Light2DUI::RenderUpdate()
    {
        if (FALSE == ComponentUI::RenderUpdate())
            return FALSE;




        return TRUE;
    }
}