#include "SatelliteViewImp.h"

char SatelliteViewImp::getObjectAt(size_t x, size_t y) const {
    if (x >= rows || y >= columns) {
        return '&';
    }

    // Check for tanks
    for (const auto& tank : game_all_tanks) {
        if (tank.getLocationX() == static_cast<int>(x) && tank.getLocationY() == static_cast<int>(y)) {
            if (tank.getPlayerId() == called_player_id && tank.getTankIndex() == called_tank_index) {
                return '%'; // Called tank
            }
            
            if(tank.getPlayerId() == 1){
                return '1'; // Player 1 tank
            }
            else if(tank.getPlayerId() == 2){
                return '2'; // Player 2 tank
            }
        }
    }

    // Check for shells
    for (const auto& shell : game_flying_shells) {
        if (shell.getLocation().first == static_cast<int>(x) && shell.getLocation().second == static_cast<int>(y)) {
            return '*'; // Shell
        }
    }

    // Check for walls
    if (game_board.isWallLocation(x, y)) {
        return '#'; // Wall
    }

    // Check for mines
    if (game_board.isMineLocation(x, y)) {
        return '@'; // Mine
    }

    return ' '; // Empty space
}

void SatelliteViewImp::setCalledPlayerId(int player_id) {
    called_player_id = player_id;
}

void SatelliteViewImp::setCalledTankIndex(int tank_index) {
    called_tank_index = tank_index;
}
