#ifndef WALL_H
#define WALL_H
#include "GameObject.h"

class Wall : public GameObject {
    private:
        int lives;
        static constexpr int DEFAULT_LIVES = 2; // Consider to put in config file

    public:
        Wall() : GameObject(), lives(DEFAULT_LIVES) {}
        void reduceLife();
        bool isDestroyed() const;   
};

#endif