#pragma once

namespace ff7r
{
	class ImGuiEventMgr : public Singleton<ImGuiEventMgr>
	{
    public:
        void Tick();

        void AddEvent(const ImguiEvent& _event) { events.push_back(_event); }

    private:
        SINGLE(ImGuiEventMgr);

        void CreateEmpty(ImguiEvent& _event);

        vector<ImguiEvent>          events;
	};
}

