#include "Wall.h"

void Wall::reduceLife() {
    lives--;
}

bool Wall::isDestroyed() const {
    return lives <= 0;
}
