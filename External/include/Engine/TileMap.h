#pragma once

#include "RenderComponent.h"


namespace ff7r
{
    class StructuredBuffer;
    class TileMap : public RenderComponent
    {
    public:
        TileMap();
        ~TileMap();

        CLONE(TileMap);

        virtual void FinalTick() override;
        virtual void Render() override;

        void UpdateData();

        void SetTileCount(UINT _cnt_x, UINT _cnt_y);
        void SetSliceSize(vec2 _slice_size) { slice_size = _slice_size; }

        virtual void SaveToLevelFile(FILE* _file) override;
        virtual void LoadFromLevelFile(FILE* _file) override;
    private:
        UINT                tile_count_x;  // Ÿ�� ����
        UINT                tile_count_y;  // Ÿ�� ����
        vec2                slice_size;   // Ÿ�� �ϳ��� ũ��(UV ����)
        vector<Tile>        tiles;
        StructuredBuffer*   buffer;
    };
}
