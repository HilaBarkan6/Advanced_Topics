#include "Tank.h"
#include "Shell.h"
#include <vector>
#include <algorithm>

Tank::Tank(int x, int y, CanonDirection direction, int id) : GameObject(), id(id), alive(true), location_x(x), location_y(y), canon_direction(direction), unused_shells_count(16) {}

Tank::~Tank() {
    for (Shell* shell : flying_shells) {
        delete shell; // Clean up the dynamically allocated Shell objects
    }
    flying_shells.clear(); // Clear the vector to avoid dangling pointers
}

int Tank::getId() const {
    return id;
}

bool Tank::getAlive(){
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

const std::vector<Shell *>& Tank::getFlyingShells() const {
    return flying_shells;
}

void::Tank::addFlyingShell(Shell * shell) {
    flying_shells.push_back(shell);
}

void Tank::deleteShell(Shell * shell) {
    auto it = std::find(flying_shells.begin(), flying_shells.end(), shell);
    if (it != flying_shells.end()) {
        flying_shells.erase(it);
        delete shell; // Clean up the dynamically allocated Shell object
    }
}






