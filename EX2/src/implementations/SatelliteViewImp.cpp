#include "SatelliteViewImp.h"

char SatelliteViewImp::getObjectAt(size_t x, size_t y) const {
    if (x >= rows || y >= columns) {
        return '&';
    }

    // Check for tanks
    // TODO - Need to give the satellite view a field to know which tank requested it, to know what char to return
    for (const auto& tank : *game_all_tanks) {
        if (tank.getLocationX() == x && tank.getLocationY() == y) {
            if(tank.getId() == 1){
                return '1'; // Player 1 tank
            }
            else if(tank.getId() == 2){
                return '2'; // Player 2 tank
            }
        }
    }

    // Check for shells
    for (const auto& shell : *game_flying_shells) {
        if (shell.getLocation().first == x && shell.getLocation().second == y) {
            return '*'; // Shell
        }
    }

    // Check for walls
    if (game_board->isWallLocation(x, y)) {
        return '#'; // Wall
    }

    // Check for mines
    if (game_board->isMineLocation(x, y)) {
        return '@'; // Mine
    }

    return ' '; // Empty space
}
