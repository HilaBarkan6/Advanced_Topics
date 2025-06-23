#include "Tank.h"

Tank::Tank(int x, int y, CanonDirection direction, int player_id, int tank_index, int shells_count, std::unique_ptr<TankAlgorithm> tank_algorithm) : 
    GameObject(),
    player_id(player_id),
    tank_index(tank_index), 
    alive(true), 
    location_x(x), 
    location_y(y), 
    canon_direction(direction), 
    unused_shells_count(shells_count), 
    tank_algorithm(std::move(tank_algorithm)) {}


int Tank::getPlayerId() const {
    return player_id;
}

int Tank::getTankIndex() const {
    return tank_index;
}

bool Tank::getAlive() const {
    return alive;
}

void Tank::setAlive(){
    alive = false;
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

CanonDirection Tank::getCanonDirection() const {
    return canon_direction;
}

void Tank::setCanonDirection(CanonDirection direction) {
    canon_direction = direction;
}


int Tank::getUnusedShellsCount() const {
    return unused_shells_count;
}

void Tank::setUnusedShellsCount(int count) {
    unused_shells_count = count;
}

TankAlgorithm& Tank::getTankAlgorithm() const {
    return *tank_algorithm;
}