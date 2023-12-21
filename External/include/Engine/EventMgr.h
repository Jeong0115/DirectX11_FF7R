#pragma once

namespace ff7r
{
    class EventMgr : public Singleton<EventMgr>
    {
    public:
        void Tick();

        void AddEvent(const Event& _evnent) { events.push_back(_evnent); }
        bool IsLevelChanged() { return is_level_changed; }

    private:
        SINGLE(EventMgr);

        void DeleteObject();      

        vector<Event>          events;
        vector<GameObject*>    delete_objects;

        bool                   is_level_changed;
    };
}
