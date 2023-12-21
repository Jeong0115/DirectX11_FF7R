#include "pch.h"
#include "CameraUI.h"

namespace ff7r
{
    CameraUI::CameraUI()
        : ComponentUI("##Camera", COMPONENT_TYPE::CAMERA)
    {
        SetName("Camera");
    }

    CameraUI::~CameraUI()
    {
    }

    int CameraUI::RenderUpdate()
    {
        if (FALSE == ComponentUI::RenderUpdate())
            return FALSE;





        return TRUE;
    }
}