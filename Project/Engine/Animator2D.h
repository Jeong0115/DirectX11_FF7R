#pragma once

#include "ptr.h"

#include "Component.h"
#include "Texture.h"

namespace ff7r
{
    class Animation2D;
    class Animator2D : public Component
    {
    public:
        Animator2D();
        ~Animator2D();

        CLONE(Animator2D);

        virtual void FinalTick() override;
        void UpdateData();
        void Clear();

        void Play(const wstring& _anim_name, bool _is_repeat);
        Animation2D* FindAnim(const wstring& _anim_name);

        void CreateAnimation(const wstring& _anim_name, Ptr<Texture> _atlas_tex, vec2 _left_top, vec2 _slice, vec2 _back_size, int _frame_cnt, int _fps);

        virtual void SaveToLevelFile(FILE* _file) override;
        virtual void LoadFromLevelFile(FILE* _file) override;

    private:
        map<wstring, Animation2D*>  anim_map;  // Animation 목록
        Animation2D*                cur_anim; // 현재 재생중인 Animation
        bool                        is_repeat;  // 반복
    };
}
