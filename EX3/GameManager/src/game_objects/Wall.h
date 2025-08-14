#ifndef WALL_H
#define WALL_H
#include "GameObject.h"

namespace GameManager_209399021_208239152 {
    class Wall : public GameObject {
        private:
            int lives;

        public:
            Wall(int lives) : GameObject(), lives(lives) {}
            void reduceLife();
            bool isDestroyed() const;   
    };
}

#endif