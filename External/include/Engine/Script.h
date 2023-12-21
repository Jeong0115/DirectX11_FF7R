#pragma once

#include "LevelMgr.h"
#include "Level.h"
#include "Layer.h"

#include "ResourceMgr.h"
#include "TimeMgr.h"
#include "KeyMgr.h"

#include "Component.h"
#include "GameObject.h"
#include "components.h"

namespace ff7r
{
    enum class SCRIPT_PARAM
    {
        INT,
        FLOAT,
        VEC2,
        VEC4,
    };

    struct ScriptParam
    {
        SCRIPT_PARAM    param;
        void*           data;
        string          desc;
    };

    class Script : public Component
    {
    public:
        Script(UINT _script_type);
        ~Script();

        void Destroy() { DestroyObject(GetOwner()); }
        void SetLifeSpan(float _time) { GetOwner()->SetLifeSpan(_time); }
        UINT GetScriptType() { return script_type; }
        const vector<ScriptParam>& GetScritpParam() { return parameters; }

        virtual void FinalTick() final {};
        virtual void BeginOverlap(Collider2D* _other) {}
        virtual void OnOverlap(Collider2D* _other) {}
        virtual void EndOverlap(Collider2D* _other) {}

        virtual void SaveToLevelFile(FILE* _file) override {}
        virtual void LoadFromLevelFile(FILE* _file) override {}

    protected:
        void AddScriptParam(SCRIPT_PARAM _param, void* _data, const string& _desc);

    private:
        UINT                    script_type;
        vector<ScriptParam>     parameters;
    };
}
