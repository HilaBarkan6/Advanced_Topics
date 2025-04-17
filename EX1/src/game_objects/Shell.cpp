#include "Shell.h"
#include "Tank.h"

Shell::Shell(std::pair<int,int> loc, CanonDirection dir, int id): GameObject(), location(loc), flying_direction(dir), tank_id(id) {};
std::pair<int,int> Shell::getLocation() const { return location; }

void Shell::setLocation(std::pair<int,int> loc) { location=loc; }
void Shell::setFlyingDirection(CanonDirection direction) { flying_direction = direction; }
std::pair<int, int> Shell::getNextLocation() const { return next_location; }
void Shell::setNextLocation(std::pair<int,int> next_loc){
    next_location = next_loc;
}
CanonDirection Shell::getFlyingDirection() const {
    return flying_direction;
}

int Shell::getTankID() const {
    return tank_id;
}

void Shell::setPrevLocation(std::pair<int, int> cur_location){
    previous_location = cur_location;
}

std::pair<int, int> Shell::getPrevLocation() const{
    return previous_location;
}