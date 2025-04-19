#include "Tank.h"
#include "Shell.h"
#include <vector>
#include <algorithm>

Tank::Tank(int x, int y, CanonDirection direction) : GameObject(), location_x(x), location_y(y), canon_direction(direction), unused_shells_count(16), alive(true) {}

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




void Tank::setCanonDirection(CanonDirection direction) {
    canon_direction = direction;
}

CanonDirection Tank::getCanonDirection() const {
    return canon_direction;
}

void Tank::setAlive(){
    alive = false;
}

bool Tank::getAlive(){
    return alive;
}

int Tank::getUnusedShellsCount() const {
    return unused_shells_count;
}

void Tank::setUnusedShellsCount(int count) {
    unused_shells_count = count;
}

std::vector <Shell *>& Tank::getFlyingShells() {
    return flying_shells;
}

void Tank::deleteShell(Shell * shell) {
    auto it = std::find(flying_shells.begin(), flying_shells.end(), shell);
    if (it != flying_shells.end()) {
        flying_shells.erase(it);
        delete shell; // Clean up the dynamically allocated Shell object
    }
}

Tank::~Tank() {
    for (Shell* shell : flying_shells) {
        delete shell; // Clean up the dynamically allocated Shell objects
    }
    flying_shells.clear(); // Clear the vector to avoid dangling pointers
}

