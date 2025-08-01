#include "SimpleTankAlgorithm.h"
using namespace Algorithm_209399021_208239152;
//REGISTER_TANK_ALGORITHM(SimpleTankAlgorithm);

ActionRequest SimpleTankAlgorithm::getAction() {
    turn_counter++;  
    // We decided to request battle info every 2 turns
    if(turn_counter % battle_info_request_period == 1){
        return ActionRequest::GetBattleInfo;
    }

    ActionRequest cur_action = action_to_apply;
    updateLocalState(cur_action);
    return cur_action;
}

void SimpleTankAlgorithm::updateBattleInfo(BattleInfo& info) {
    SimpleBattleInfo& simple_info = dynamic_cast<SimpleBattleInfo&>(info); // Downcast is allowed

    if(height == 0 && width == 0){
        height = simple_info.getHeight();
        width = simple_info.getWidth();
    }

    // Find closest enemy tank
    std::pair<int, int> my_location = simple_info.getCalledTankLocation();
    std::vector<std::pair<int, int>> enemy_tanks = (player_id==1) ? simple_info.getTanks2Locations() : simple_info.getTanks1Locations();
    std::pair<int, int> closest_enemy = getClosestEnemyTank(my_location, enemy_tanks);

    // If there are no enemy tanks, we can just move forward or rotate
    if(closest_enemy.first == -1 && closest_enemy.second == -1){
        std::pair<int, int> next_location = getNextForwardLocation(my_location, current_canon_direction);
        if(canMove(next_location.first, next_location.second, simple_info.getWallsLocations(), simple_info.getMinesLocations(), simple_info.getTanks1Locations(), simple_info.getTanks2Locations())){
            action_to_apply = ActionRequest::MoveForward;
        }
        else{
            action_to_apply = ActionRequest::RotateRight90;
        }
    }
    // If there is an enemy tank, we will try to shoot it.
    else{
        if (canShoot(height, width, my_location, closest_enemy, current_canon_direction, simple_info.getWallsLocations()) && (last_shoot_turn == -1 || turn_counter - last_shoot_turn >= shooting_waiting_turns)) {
            action_to_apply = ActionRequest::Shoot;
        }
        else {
            std::pair<int, int> next_location = getNextForwardLocation(my_location, current_canon_direction);
            if(canMove(next_location.first, next_location.second, simple_info.getWallsLocations(), simple_info.getMinesLocations(), simple_info.getTanks1Locations(), simple_info.getTanks2Locations())){
                action_to_apply = ActionRequest::MoveForward;
            }
            else{
                action_to_apply = ActionRequest::RotateRight90;
            }
        }
    }
}