#include "PlayerImp.h"

void PlayerImp::putLocationByChar(char object_sign, int x, int y) {
    switch (object_sign) {
        case '1': // Tank1
            battle_info.addTanks1Location(std::make_pair(x, y));
            break;
        case '2': // Tank2
            battle_info.addTanks2Location(std::make_pair(x, y));
            break;
        case '%': // Called tank
            battle_info.addCalledTankLocation(std::make_pair(x, y));
            break;
        case '*': // Shell
            battle_info.addShellLocation(std::make_pair(x, y));
            break;
        case '@': // Mine
            battle_info.addMineLocation(std::make_pair(x, y));
            break;
        case '#': // Wall
            battle_info.addWallLocation(std::make_pair(x, y));
            break;
        default:
            break; // Ignore other objects
    }
}