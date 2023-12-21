#pragma once


namespace ff7r
{
    class Level;
    class GameObject;
    class LevelMgr : public Singleton<LevelMgr>
    {
        SINGLE(LevelMgr);

    public:
        void Init();
        void Tick();

        Level* GetCurLevel() { return cur_level; }
        GameObject* FindObjectByName(const wstring& _name);
        void FindObjectByName(const wstring& _name, vector<GameObject*>& _result);

        void ChangeLevel(Level* _next_level);

    private:
        Level* cur_level;

    };

}