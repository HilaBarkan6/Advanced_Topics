#include "TankAlgorithmImp.h"

TankAlgorithmImp::TankAlgorithmImp(int player_id, int tank_index): player_id(player_id), tank_index(tank_index) {}
int TankAlgorithmImp::getPlayerId() {
    return player_id;
}
int TankAlgorithmImp::getTankIndex() {
    return tank_index;
}
