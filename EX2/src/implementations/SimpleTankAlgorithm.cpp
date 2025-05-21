#include "SimpleTankAlgorithm.h"
#include "SimpleBattleInfo.h"
#include <iostream>

SimpleTankAlgorithm::SimpleTankAlgorithm(int player_id, int tank_index): player_id(player_id), tank_index(tank_index) {
    current_canon_direction = (player_id == 1) ? CanonDirection::LEFT : CanonDirection::RIGHT;
}

ActionRequest SimpleTankAlgorithm::getAction() {
    turn_counter++;
    if(turn_counter%3 == 1){
        return ActionRequest::GetBattleInfo;
    }

    switch (actions_to_apply[0]) {
    
        case ActionRequest::RotateLeft45:
            current_canon_direction = rotate(current_canon_direction, -1);
            break;
        
        case ActionRequest::RotateRight45:
            current_canon_direction = rotate(current_canon_direction, 1);
            break;

        case ActionRequest::RotateLeft90:   
            current_canon_direction = rotate(current_canon_direction, -2);
            break;

        case ActionRequest::RotateRight90:      
            current_canon_direction = rotate(current_canon_direction, 2);
            break;

        default:
            break;
        
    }
    return actions_to_apply[0];
    actions_to_apply.erase(actions_to_apply.begin());  
} 

//TODO - we decide for now that the bfs logic is in this function and that getAction is very simple, we should think and maybe ask if thats a good idea.  
void SimpleTankAlgorithm::updateBattleInfo(BattleInfo& info) {
    
    SimpleBattleInfo& simple_info = dynamic_cast<SimpleBattleInfo&>(info);
    
    std::cout << simple_info.getCalledTankLocation().first << " " << simple_info.getCalledTankLocation().second << std::endl;
    std::cout << "SimpleTankAlgorithm: updateBattleInfo called" << std::endl;

    // if there is a shell very close to us, we should try to run away avoiding walls and mines
    // find closest enemy tank
    // calculate next 2 action to get to this enemy, avoid walls and mines and shooting our tanks
    // save those 2 steps in the actions vector
    
}

int SimpleTankAlgorithm::getPlayerId() {
    return player_id;
} 

int SimpleTankAlgorithm::getTankIndex(){
    return tank_index;
}

CanonDirection SimpleTankAlgorithm::rotate(CanonDirection cur_dir, int rotation) {
    // Used ChatGpt to cast the direction to number and preforme rotation as addition/substraction
    int new_dir = static_cast<int>(cur_dir) + rotation;
    if (new_dir < 0) {
        new_dir += 8; // Wrap around to the last direction
    } else if (new_dir >= 8) {
        new_dir -= 8; // Wrap around to the first direction
    }
    return static_cast<CanonDirection>(new_dir);
}