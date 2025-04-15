#include "Shell.h"
#include "Tank.h"

Shell::Shell(int x, int y, CanonDirection dir): GameObject(), location_x(x), location_y(y), flying_direction(dir) {};
int Shell::getLocationX() const { return location_x; }
int Shell::getLocationY() const { return location_y; }
void Shell::setLocation(int x, int y) { location_x = x; location_y = y; }
void Shell::setFlyingDirection(CanonDirection direction) { flying_direction = direction; }
std::pair<int, int> Shell::getNextLocation() const { return next_location; }
void Shell::setNextLocation(int x, int y){
    next_location.first = x;
    next_location.second = y;
}
CanonDirection Shell::getFlyingDirection() const {
    return flying_direction;
}