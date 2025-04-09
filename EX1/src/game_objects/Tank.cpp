#include "Tank.h"
#include "Shell.h"
#include <vector>

Tank::Tank(int x, int y, CanonDirection direction) : GameObject() ,location_x(x), location_y(y), canon_direction(direction), last_shoot_turn_index(-1), last_backwards_turn_index(-1) {
    for(int i = 0; i < 16; i++){
        unused_shells[i] = Shell();
    }
}

int Tank::getLocationX() const {
    return location_x;
}

int Tank::getLocationY() const {
    return location_y;
}

void Tank::setLocation(int x, int y) {
    location_x = x;
    location_y = y;
}

void Tank::setLastShootTurnIndex(int index) {
    last_shoot_turn_index = index;
}

int Tank::getLastShootTurnIndex() const {
    return last_shoot_turn_index;
}

void Tank::setLastBackwardsTurnIndex(int index) {
    last_backwards_turn_index = index;
}

int Tank::getLastBackwardsTurnIndex() const {
    return last_backwards_turn_index;
}

void Tank::setCanonDirection(CanonDirection direction) {
    canon_direction = direction;
}

CanonDirection Tank::getCanonDirection() const {
    return canon_direction;
}