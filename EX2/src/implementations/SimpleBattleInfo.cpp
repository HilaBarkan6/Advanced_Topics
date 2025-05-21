#include "SimpleBattleInfo.h"
#include <utility> // for std::pair
#include <vector>

int SimpleBattleInfo::getHeight() const {
    return height;
}
int SimpleBattleInfo::getWidth() const {
    return width;
}
const std::vector<std::pair<int, int>>& SimpleBattleInfo::getTanks1Locations() const {
    return tanks1_locations;
}
const std::vector<std::pair<int, int>>& SimpleBattleInfo::getTanks2Locations() const {
    return tanks2_locations;
}
const std::pair<int, int>& SimpleBattleInfo::getCalledTankLocation() const {
    return called_tank_location;
}
const std::vector<std::pair<int, int>>& SimpleBattleInfo::getShellsLocations() const {
    return shells_locations;
}
const std::vector<std::pair<int, int>>& SimpleBattleInfo::getMinesLocations() const {
    return mines_locations;
}
const std::vector<std::pair<int, int>>& SimpleBattleInfo::getWallsLocations() const {
    return walls_locations;
}
void SimpleBattleInfo::setHeight(int height) {
    height = height;
}
void SimpleBattleInfo::setWidth(int width) {
    width = width;
}
void SimpleBattleInfo::setTanks1Locations(const std::vector<std::pair<int, int>>& tanks1) {
    tanks1_locations = tanks1;
}
void SimpleBattleInfo::setTanks2Locations(const std::vector<std::pair<int, int>>& tanks2) {
    tanks2_locations = tanks2;
}
void SimpleBattleInfo::setCalledTankLocation(const std::pair<int, int>& location) {
    called_tank_location = location;
}
void SimpleBattleInfo::setShellsLocations(const std::vector<std::pair<int, int>>& shells) {
    shells_locations = shells;
}
void SimpleBattleInfo::setMinesLocations(const std::vector<std::pair<int, int>>& mines) {
    mines_locations = mines;
}
void SimpleBattleInfo::setWallsLocations(const std::vector<std::pair<int, int>>& walls) {
    walls_locations = walls;
}