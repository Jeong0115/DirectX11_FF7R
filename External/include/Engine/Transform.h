#pragma once
#include "Component.h"

namespace ff7r
{
    class Transform : public Component
    {
    public:
        Transform();
        ~Transform();

        CLONE(Transform);

        virtual void FinalTick() override;
        void UpdateData();

        void SetRelativePos(vec3 _pos)     { relative_pos = _pos; }
        void SetRelativeScale(vec3 _scale) { relative_scale = _scale; }
        void SetRelativeRot(vec3 _rotation)     { relative_rotation = _rotation; }
        void SetRelativePos(float _x, float _y, float _z)   { relative_pos = vec3(_x, _y, _z); }
        void SetRelativeScale(float _x, float _y, float _z) { relative_scale = vec3(_x, _y, _z); }
        void SetRelativeRot(float _x, float _y, float _z)   { relative_rotation = vec3(_x, _y, _z); }

        vec3 GetRelativePos() const     { return relative_pos; }
        vec3 GetRelativeScale() const   { return relative_scale; }
        vec3 GetRelativeRot() const     { return relative_rotation; }
        vec3 GetRelativeDir(DIR_TYPE _type) const   { return relative_dir[(UINT)_type]; }
        vec3 GetWorldDir(DIR_TYPE _type) const      { return world_dir[(UINT)_type]; }
        vec3 GetWorldPos() { return mat_world.Translation(); }

        // ��� �̵�, ũ�⸦ ���밪���� ����  
        void SetAbsolute(bool _set) { absolute = _set; }

        const Matrix& GetWorldScaleMat() { return mat_world_scale; }
        const Matrix& GetWorldMat() const { return mat_world; }
        void SetWorldMat(const Matrix& _mat) { mat_world = _mat; }

        virtual void SaveToLevelFile(FILE* _file) override;
        virtual void LoadFromLevelFile(FILE* _file) override;

    private:
        vec3    relative_pos;
        vec3    relative_scale;
        vec3    relative_rotation;
        vec3    relative_dir[3];
        vec3    world_dir[3];

        Matrix  mat_world; // ũ��, ȸ��, �̵� ������ ���ĳ���   
        Matrix  mat_world_scale; // ���� ũ�� ���

        bool    absolute;    // ��� �̵�, ũ�⸦ ���밪���� ����    
    };
}
