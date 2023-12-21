#pragma once
#include "UI.h"
namespace ff7r
{
    class SearchListUI : public UI
    {
    public:
        enum class SearchListType
        {
            COMPONENTS,
            MATERIALS,
            MESHES,
            TEXTURES,
            SOUNDS,
            END
        };

        SearchListUI();
        virtual ~SearchListUI();

        virtual void FinalTick() override;
        virtual int RenderUpdate() override;
        void Reset(SearchListType _type, ImVec2 _size);

        void AddItem(const string& _str_item) { string_data.push_back(_str_item); }
        void Clear() { string_data.clear(); }

        void AddDynamic_Select(UI* _inst, UI_DELEGATE_1 _func) { select_inst = _inst; select_delegate = _func; }

    private:
        void Close();

        void AddComponents();
        void AddMaterials();
        void AddTextures();
        void AddMeshes();
        void AddSounds();

        vector<string>          string_data;

        UI*                     select_inst;
        UI_DELEGATE_1           select_delegate;

        char                    search_buff[256];
        bool                    is_first_frame;
    };

}