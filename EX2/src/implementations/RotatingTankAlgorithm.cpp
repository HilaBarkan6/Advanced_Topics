#include "RotatingTankAlgorithm.h"
#include "SimpleBattleInfo.h"
#include <iostream>

RotatingTankAlgorithm::RotatingTankAlgorithm(int player_id, int tank_index): player_id(player_id), tank_index(tank_index) {}
ActionRequest RotatingTankAlgorithm::getAction() {
    // TODO - implement the logic to decide the action
    return ActionRequest::RotateLeft45;
}   
void RotatingTankAlgorithm::updateBattleInfo(BattleInfo& info) {
    // TODO - implement the logic to update the battle info
    // do something with info just so it won't be unused
    SimpleBattleInfo& simple_info = dynamic_cast<SimpleBattleInfo&>(info);
    std::cout << simple_info.getCalledTankLocation().first << " " << simple_info.getCalledTankLocation().second << std::endl;
    std::cout << "RotatingTankAlgorithm: updateBattleInfo called" << std::endl;
}

int RotatingTankAlgorithm::getPlayerId() {
    return player_id;
} 

int RotatingTankAlgorithm::getTankIndex(){
    return tank_index;
}
