#include "BFSTankAlgorithm.h"

using namespace Algorithm_209399021_208239152;
REGISTER_TANK_ALGORITHM(BFSTankAlgorithm);

//TODO - fix the 10
BFSTankAlgorithm::BFSTankAlgorithm(int player_id, int tank_index)
        : TankAlgorithmImp(player_id, tank_index), 
        bfs_max_depth(10) {}

ActionRequest BFSTankAlgorithm::getAction() {
    turn_counter++;
    
    // We decided to request battle info every 3 turns, so if the turn counter is 1, we request battle info
    if(turn_counter % BATTLE_INFO_REQUEST_PERIOD == 1){
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

    // Compare with previous_battle_info if exists
    if(prev_shells_locations.size() > 0) {
        // Analyze the changes in the battle info
        analyzeShellsMovements(simple_info.getShellsLocations());
    }

    prev_shells_locations = simple_info.getShellsLocations(); // Update previous shells locations for next comparison
    last_info_request_turn = turn_counter; // Update the last info request turn

    if(height == 0 && width == 0){
        height = simple_info.getHeight();
        width = simple_info.getWidth();
    }

    std::pair<int, int> my_location = simple_info.getCalledTankLocation();
    std::vector<std::pair<int, int>> enemy_tanks = (player_id==1) ? simple_info.getTanks2Locations() : simple_info.getTanks1Locations();
    std::pair<int, int> closest_enemy = getClosestEnemyTank(my_location, enemy_tanks); // Get the closest enemy tank location

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

void BFSTankAlgorithm::analyzeShellsMovements(std::vector<std::pair<int, int>> cur_shells_locations) {
    shells_movements.clear(); // Clear previous movements map

    // For each shell in the current state, try to find matching shell in previous state to infer movement vector
    for (const auto& curr_shell : cur_shells_locations) {
        bool matched = false;
        for (const auto& prev_shell : prev_shells_locations) {
            int dx = curr_shell.first - prev_shell.first;
            int dy = curr_shell.second - prev_shell.second;

            // We expect shell to move 2*(diff between turn_counter and last_info_request_turn ) 
            int diff = 2*(turn_counter-last_info_request_turn);
            if (std::abs(dx) == diff || std::abs(dy) == diff) { 
                // Found a matching projectile, record its movement vector
                shells_movements[curr_shell] = {dx, dy};
                matched = true;
                break;
            }
        }
        if (!matched) {
            // New shell — no previous position found
            shells_movements[curr_shell] = {0, 0}; // zero vector means "new shell" or unknown movement
        }
    }
}

// std::pair<std::set<std::pair<int, int>>, std::set<std::pair<int, int>>> BFSTankAlgorithm::computeDangerPositions() const {
//     // set of dangerous positions for the first turn after the info request
//     std::set<std::pair<int, int>> first_danger_positions;
//     // set of dangerous positions for the second turn after the info request
//     std::set<std::pair<int, int>> second_danger_positions;

//     for (const auto& [pos, vec] : shells_movements) {
//         if (vec.first == 0 && vec.second == 0) continue; // new shell

//         int x = pos.first;
//         int y = pos.second;
//         int dx = (vec.first > 0) ? 1 : ((vec.first == 0) ? 0 : -1);
//         int dy = (vec.second > 0) ? 1 : ((vec.second == 0) ? 0 : -1);

//         first_danger_positions.insert({x + 2*dx, y + 2*dy});
//         first_danger_positions.insert({x + 3*dx, y + 3*dy});
//         first_danger_positions.insert({x + 4*dx, y + 4*dy});
//         second_danger_positions.insert({x + 4*dx, y + 4*dy});
//         second_danger_positions.insert({x + 5*dx, y + 5*dy});
//         second_danger_positions.insert({x + 6*dx, y + 6*dy});

//     }

//     return std::make_pair(first_danger_positions, second_danger_positions);
// }

std::pair<std::set<std::pair<int, int>>, std::set<std::pair<int, int>>> BFSTankAlgorithm::computeDangerPositions() const {
    constexpr int DANGER_FIRST_TURN_START_STEP = 2;
    constexpr int DANGER_FIRST_TURN_END_STEP = 4;

    constexpr int DANGER_SECOND_TURN_START_STEP = 4;
    constexpr int DANGER_SECOND_TURN_END_STEP = 6;

    std::set<std::pair<int, int>> first_danger_positions;
    std::set<std::pair<int, int>> second_danger_positions;

    for (const auto& [pos, vec] : shells_movements) {
        if (vec.first == 0 && vec.second == 0) continue; // new shell

        int x = pos.first;
        int y = pos.second;
        int dx = (vec.first > 0) ? 1 : ((vec.first == 0) ? 0 : -1);
        int dy = (vec.second > 0) ? 1 : ((vec.second == 0) ? 0 : -1);

        // Add positions for the first turn
        for (int step = DANGER_FIRST_TURN_START_STEP; step <= DANGER_FIRST_TURN_END_STEP; ++step) {
            first_danger_positions.emplace(x + dx * step, y + dy * step);
        }

        // Add positions for the second turn
        for (int step = DANGER_SECOND_TURN_START_STEP; step <= DANGER_SECOND_TURN_END_STEP; ++step) {
            second_danger_positions.emplace(x + dx * step, y + dy * step);
        }
    }

    return {first_danger_positions, second_danger_positions};
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

void BFSTankAlgorithm::tryMoveForward(const QueueNode& current, const SimpleBattleInfo& info, std::pair<std::set<std::pair<int, int>>, std::set<std::pair<int, int>>>& danger_positions) {
    auto [x, y] = getNextForwardLocation({current.state.x, current.state.y}, current.state.dir);

    //if (danger_positions.count({x, y})) return; // Unsafe move, location is dangerous because shells are moving there

    if (canMove(x, y, info.getWallsLocations(), info.getMinesLocations(), info.getTanks1Locations(), info.getTanks2Locations())) {
        State new_state = {x, y, current.state.dir};
        if (visited.find(new_state) == visited.end()) {
            visited.insert(new_state);
            ActionRequest act1 = (current.depth == 0 && (danger_positions.first).count({x,y})==0) ? ActionRequest::MoveForward : current.firstAction;
            ActionRequest act2 = (current.depth == 1 && (danger_positions.second).count({x,y})==0) ? ActionRequest::MoveForward : current.secondAction;
            q.push(QueueNode{new_state, act1, act2, current.depth + 1});
        }
    }
}

void BFSTankAlgorithm::tryRotations(const QueueNode& current) {
    static const std::vector<std::pair<ActionRequest, int>> rotations = {
        {ActionRequest::RotateLeft45,  -1},
        {ActionRequest::RotateLeft90,  -2},
        {ActionRequest::RotateRight45, 1},
        {ActionRequest::RotateRight90, 2},
    };

    for (const auto& [rotation_act, new_dir] : rotations) {
        State new_state = {current.state.x, current.state.y, rotate(current.state.dir, new_dir)};
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
    
    auto danger_positions = computeDangerPositions();
    CanonDirection dir = current_canon_direction;
    State start_state = {simple_info.getCalledTankLocation().first, simple_info.getCalledTankLocation().second, dir};
    q.push(QueueNode{start_state, ActionRequest::DoNothing, ActionRequest::DoNothing, 0});
    visited.insert(start_state);

    while(!q.empty()){
        QueueNode current = q.front();
        q.pop();
        if(current.depth > bfs_max_depth) break;

        // Check for shooting opportunity
        if (tryShoot(current, simple_info, enemy_location)) return;

        // Try move forward
        tryMoveForward(current, simple_info, danger_positions);

        // Try rotation options
        tryRotations(current);
    }

    // fallback if nothing was found
    actions_to_apply.push_back(ActionRequest::RotateLeft45);
    actions_to_apply.push_back(ActionRequest::RotateLeft45);
}