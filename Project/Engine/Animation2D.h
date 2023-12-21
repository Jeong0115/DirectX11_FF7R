#pragma once

#include "ptr.h"

#include "Entity.h"
#include "Texture.h"

namespace ff7r
{
    class Animator2D;
    class Animation2D : public Entity
    {
    public:
        Animation2D();
        ~Animation2D();

        CLONE(Animation2D);

        void FinalTick();
        void Create(const wstring& _anim_name, Ptr<Texture> _atlas_tex, vec2 _left_top, vec2 _slice, vec2 _back_size, int _frame_cnt, int _fps);

        const Anim2DFrame& GetCurFrame() { return frames[cur_frame_idx]; }
        vec2 GetBackSize() { return back_size; }
        Ptr<Texture> GetAtlasTex() { return atlas_tex; }

        bool IsFinish() { return is_finish; }
        void Reset()
        {
            cur_frame_idx = 0;
            time = 0.f;
            is_finish = false;
        }

        void SaveToLevelFile(FILE* _file);
        void LoadFromLevelFile(FILE* _file);

    private:
        Animator2D*             owner;
        vector<Anim2DFrame>     frames;
        vec2                    back_size;
        Ptr<Texture>            atlas_tex;

        float                   time;                             
        int                     cur_frame_idx;
        bool                    is_finish;

        friend class Animator2D;
    };
}
