#include "pch.h"
#include "SoundUI.h"
namespace ff7r
{
    SoundUI::SoundUI()
        : ResourceUI(RES_TYPE::SOUND)
    {
        SetName("Sound");
    }

    SoundUI::~SoundUI()
    {
    }

    int SoundUI::RenderUpdate()
    {
        ResourceUI::RenderUpdate();

        return 0;
    }
}