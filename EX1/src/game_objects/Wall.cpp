#include "Wall.h"

void Wall::hits() {
    lives--;
}

bool Wall::isDestroyed() const {
    return lives <= 0;
}