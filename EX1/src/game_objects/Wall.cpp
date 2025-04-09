#include "Wall.h"

Wall::Wall() : lives(2) {}

void Wall::hits() {
    lives--;
}

bool Wall::isDestroyed() const {
    return lives <= 0;
}