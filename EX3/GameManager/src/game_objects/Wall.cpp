#include "Wall.h"

using namespace GameManager_209399021_208239152;

void Wall::reduceLife() {
    lives--;
}

bool Wall::isDestroyed() const {
    // This threshold is specific to this function, so we define it locally to avoid polluting the class/global scope.
    constexpr int DESTRUCTION_LIFE_THRESHOLD = 0;
    return lives <= DESTRUCTION_LIFE_THRESHOLD;
}
