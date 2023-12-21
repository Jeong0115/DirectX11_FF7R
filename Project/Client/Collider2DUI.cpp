#include "pch.h"
#include "Collider2DUI.h"

namespace ff7r
{
    Collider2DUI::Collider2DUI()
        : ComponentUI("##Coillider2D", COMPONENT_TYPE::COLLIDER2D)
    {
        SetName("Coillider2D");
    }

    Collider2DUI::~Collider2DUI()
    {
    }

    int Collider2DUI::RenderUpdate()
    {
        if (FALSE == ComponentUI::RenderUpdate())
            return FALSE;




        return TRUE;
    }
}