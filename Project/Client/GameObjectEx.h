#pragma once

#include <Engine\GameObject.h>



namespace ff7r
{
    class GameObjectEx :
        public GameObject
    {
    private:

    public:
        virtual void FinalTick() override;
    };

}