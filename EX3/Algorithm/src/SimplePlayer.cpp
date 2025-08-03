
#include "SimplePlayer.h"
using namespace Algorithm_209399021_208239152;

REGISTER_PLAYER(SimplePlayer);

namespace Algorithm_209399021_208239152 {

void SimplePlayer::updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& satellite_view) {
    prev_battle_info = battle_info; // Store the previous battle info for future comparisons
    battle_info.clearInfo();

    // Iterate over the board using satellite_view and populate battle_info
    for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) {
            char object_sign = satellite_view.getObjectAt(x, y);
            putLocationByChar(object_sign, x, y);
        }
    }
    
    // Update the tank algorithm with the populated battle info 
    tank.updateBattleInfo(battle_info);
}


}