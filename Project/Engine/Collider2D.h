#pragma once
#include "Component.h"

namespace ff7r
{
    class Collider2D : public Component
    {
    public:
        Collider2D();
        ~Collider2D();

        CLONE(Collider2D);

        virtual void FinalTick() override;

        void SetOffsetPos(vec2 _offset_pos) { offset_pos = vec3(_offset_pos.x, _offset_pos.y, 0.f); }
        void SetOffsetScale(vec2 _offset_scale) { offset_scale = vec3(_offset_scale.x, _offset_scale.y, 1.f); }
        void SetAbsolute(bool _absolute) { is_absolute = _absolute; }
        void SetCollider2DType(COLLIDER2D_TYPE _type) { type = _type; }

        void BeginOverlap(Collider2D* _other);
        void OnOverlap(Collider2D* _other);
        void EndOverlap(Collider2D* _other);

        virtual void SaveToLevelFile(FILE* _file) override;
        virtual void LoadFromLevelFile(FILE* _file) override;

        const Matrix& GetColliderWorldMat() { return mat_world_colli; }

    private:
        vec3            offset_pos;
        vec3            offset_scale;
        bool            is_absolute;
        COLLIDER2D_TYPE type;
        Matrix          mat_world_colli;    // Collider 의 월드행렬

        int             collision_cnt;  // 충돌 횟수
    };
}
