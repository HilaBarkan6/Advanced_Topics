#include "Wall.h"

void Wall::reduceLife() {
    lives--;
}

bool Wall::isDestroyed() const {
    constexpr int DESTRUCTION_LIFE_THRESHOLD = 0;
    return lives <= DESTRUCTION_LIFE_THRESHOLD;
}
