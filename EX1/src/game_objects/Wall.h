#ifndef WALL_H
#define WALL_H
#include "GameObject.h"

class Wall : public GameObject
{
    private:
        int lives;
    public:
        Wall() : GameObject(), lives(2) {}
        void hits();
        bool isDestroyed() const;
        //~Wall() override = default;
   
};


#endif