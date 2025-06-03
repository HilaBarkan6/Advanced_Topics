#include "RotatingTankAlgorithm.h"
#include "SimpleBattleInfo.h"
#include <iostream>

RotatingTankAlgorithm::RotatingTankAlgorithm(int player_id, int tank_index): player_id(player_id), tank_index(tank_index), turn_counter(0)  {} // magic number 0 ? 
ActionRequest RotatingTankAlgorithm::getAction() {
    // TODO - This is relevant for input_j, on other tests just rotate
    // turn_counter++;
    // if(turn_counter < 3){
    //     return ActionRequest::RotateRight90; 
    // }
    // return ActionRequest::MoveForward;
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
