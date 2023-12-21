#pragma once
#include "ComponentUI.h"

#include <Engine\Script.h>
namespace ff7r
{
    class ScriptUI :
        public ComponentUI
    {
    private:
        Script* m_pTargetScript;
        string      m_strScriptName;

    public:
        virtual int RenderUpdate() override;

    public:
        void SetScript(Script* _Script);

    public:
        ScriptUI();
        ~ScriptUI();
    };

}