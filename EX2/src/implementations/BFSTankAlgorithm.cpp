#include "BFSTankAlgorithm.h"


ActionRequest BFSTankAlgorithm::getAction() {
    turn_counter++;
    
    // We decided to request battle info every 3 turns, so if the turn counter is 1, we request battle info
    if(turn_counter % battle_info_request_period == 1){
        return ActionRequest::GetBattleInfo;
    }

    ActionRequest cur_action = actions_to_apply[0];
    updateLocalState(cur_action);
    actions_to_apply.erase(actions_to_apply.begin());
    return cur_action;
} 

/* We decided that updateBattleInfo is a complex function that calculates the next 2 actions.
*  GetAction will return the result of the calculation.
*/
void BFSTankAlgorithm::updateBattleInfo(BattleInfo& info) {
    actions_to_apply.clear();
    SimpleBattleInfo& simple_info = dynamic_cast<SimpleBattleInfo&>(info); // Downcast is allowed

    if(height == 0 && width == 0){
        height = simple_info.getHeight();
        width = simple_info.getWidth();
    }

    // Find closest enemy tank
    std::pair<int, int> my_location = simple_info.getCalledTankLocation();
    std::vector<std::pair<int, int>> enemy_tanks = (player_id==1) ? simple_info.getTanks2Locations() : simple_info.getTanks1Locations();
    std::pair<int, int> closest_enemy = getClosestEnemyTank(my_location, enemy_tanks);

    // If closest enemy is not found, try to move using the function can_move and if not possible rotate
    if(closest_enemy.first == -1 && closest_enemy.second == -1){
        // Check if can move forward
        std::pair<int, int> next_location = getNextForwardLocation(my_location, current_canon_direction);
        if(canMove(next_location.first, next_location.second, simple_info.getWallsLocations(), simple_info.getMinesLocations(), simple_info.getTanks1Locations(), simple_info.getTanks2Locations())){
            actions_to_apply.push_back(ActionRequest::MoveForward);
            actions_to_apply.push_back(ActionRequest::GetBattleInfo);
        }
        else{
            actions_to_apply.push_back(ActionRequest::RotateRight45);
            actions_to_apply.push_back(ActionRequest::GetBattleInfo);
        }
    }
    else{
        /* Calculate next 2 action to get to this enemy, avoid walls and mines and shooting our tanks
        *  save those 2 steps in the actions vector */
        bfs(closest_enemy, simple_info);
    }
   
}

bool BFSTankAlgorithm::tryShoot(const QueueNode& current, const SimpleBattleInfo& info, const std::pair<int, int>& enemy_location) {
    int x = current.state.x;
    int y = current.state.y;
    CanonDirection dir = current.state.dir;

    if (canShoot(info.getHeight(), info.getWidth(), {x, y}, enemy_location, dir, info.getWallsLocations())) {
        ActionRequest first = current.firstAction;
        ActionRequest second = current.secondAction;

        if (current.depth == 0 && (last_shoot_turn == -1 || turn_counter - last_shoot_turn >= shooting_waiting_turns)) {
            first = ActionRequest::Shoot;
        }
        else if (current.depth == 1 && (last_shoot_turn == -1 || turn_counter - last_shoot_turn >= shooting_waiting_turns-1)) {
            second = ActionRequest::Shoot;
        }

        actions_to_apply.push_back(first);
        actions_to_apply.push_back(second);
        
        return true;
    }
    return false;
}

void BFSTankAlgorithm::tryMoveForward(const QueueNode& current, const SimpleBattleInfo& info) {
    auto [x, y] = getNextForwardLocation({current.state.x, current.state.y}, current.state.dir);

    if (canMove(x, y, info.getWallsLocations(), info.getMinesLocations(), info.getTanks1Locations(), info.getTanks2Locations())) {
        State new_state = {x, y, current.state.dir};
        if (visited.find(new_state) == visited.end()) {
            visited.insert(new_state);
            ActionRequest act1 = current.depth == 0 ? ActionRequest::MoveForward : current.firstAction;
            ActionRequest act2 = current.depth == 1 ? ActionRequest::MoveForward : current.secondAction;
            q.push(QueueNode{new_state, act1, act2, current.depth + 1});
        }
    }
}

void BFSTankAlgorithm::tryRotations(const QueueNode& current) {
    static const std::vector<std::pair<ActionRequest, CanonDirection>> rotations = {
        {ActionRequest::RotateLeft45,  CanonDirection((static_cast<int>(current.state.dir) - 1) % 8)},
        {ActionRequest::RotateLeft90,  CanonDirection((static_cast<int>(current.state.dir) - 2) % 8)},
        {ActionRequest::RotateRight45, CanonDirection((static_cast<int>(current.state.dir) + 1) % 8)},
        {ActionRequest::RotateRight90, CanonDirection((static_cast<int>(current.state.dir) + 2) % 8)},
    };

    for (const auto& [rotation_act, new_dir] : rotations) {
        State new_state = {current.state.x, current.state.y, new_dir};
        if (visited.find(new_state) == visited.end()) {
            visited.insert(new_state);
            ActionRequest act1 = current.depth == 0 ? rotation_act : current.firstAction;
            ActionRequest act2 = current.depth == 1 ? rotation_act : current.secondAction;
            q.push(QueueNode{new_state, act1, act2, current.depth + 1});
        }
    }
}

void BFSTankAlgorithm::bfs(const std::pair<int, int>& enemy_location, const SimpleBattleInfo& simple_info ){
    visited.clear();
    q = std::queue<QueueNode>();
    actions_to_apply.clear();

    CanonDirection dir = current_canon_direction;
    State start_state = {simple_info.getCalledTankLocation().first, simple_info.getCalledTankLocation().second, dir};
    q.push(QueueNode{start_state, ActionRequest::DoNothing, ActionRequest::DoNothing, 0});
    visited.insert(start_state);

    while(!q.empty()){
        QueueNode current = q.front();
        q.pop();
        if(current.depth > 10) break;

        // Check for shooting opportunity
        if (tryShoot(current, simple_info, enemy_location)) return;

        // Move forward
        tryMoveForward(current, simple_info);

        // Rotation options
        tryRotations(current);
    }

    // fallback if nothing was found
    actions_to_apply.push_back(ActionRequest::RotateLeft45);
    actions_to_apply.push_back(ActionRequest::RotateLeft45);
}

