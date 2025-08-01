#include "PlayerImp.h"
using namespace Algorithm_209399021_208239152;

PlayerImp::PlayerImp(int , size_t x, size_t y, size_t , size_t )
    : battle_info(x, y), width(x), height(y),
        out_of_bounds_sign(Config::getInstance().get("out_of_bounds_sign", std::string(1, OUT_OF_BOUNDS_SIGN))[0]),
        wall_sign(Config::getInstance().get("wall_sign", std::string(1, WALL_SIGN))[0]),
        tank1_sign(Config::getInstance().get("tank1_sign", std::string(1, TANK1_SIGN))[0]),
        tank2_sign(Config::getInstance().get("tank2_sign", std::string(1, TANK2_SIGN))[0]),
        called_tank_sign(Config::getInstance().get("called_tank_sign", std::string(1, CALLED_TANK_SIGN))[0]),
        shell_sign(Config::getInstance().get("shell_sign", std::string(1, SHELL_SIGN))[0]),
        mine_sign(Config::getInstance().get("mine_sign", std::string(1, MINE_SIGN))[0]) {}

void PlayerImp::putLocationByChar(char object_sign, int x, int y) {
    std::pair<int, int> location = {x, y};

    if (object_sign == tank1_sign) {
        battle_info.addTanks1Location(location);
    } 
    else if (object_sign == tank2_sign) {
        battle_info.addTanks2Location(location);
    } 
    else if (object_sign == called_tank_sign) {
        battle_info.addCalledTankLocation(location);
    } 
    else if (object_sign == shell_sign) {
        battle_info.addShellLocation(location);
    } 
    else if (object_sign == mine_sign) {
        battle_info.addMineLocation(location);
    } 
    else if (object_sign == wall_sign) {
        battle_info.addWallLocation(location);
    }
}