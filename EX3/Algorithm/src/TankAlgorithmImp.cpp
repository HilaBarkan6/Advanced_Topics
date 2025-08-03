#include "TankAlgorithmImp.h"
#include <limits>
#include <cstdlib>
using namespace Algorithm_209399021_208239152;

//TODO - fix the 4
TankAlgorithmImp::TankAlgorithmImp(int player_id, int tank_index): player_id(player_id), tank_index(tank_index), 
                turn_counter(0), last_info_request_turn(-1), height(0), width(0),
                 shooting_waiting_turns(4), 
                 last_shoot_turn(-1) {

        current_canon_direction = (player_id == 1) ? CanonDirection::LEFT : CanonDirection::RIGHT;
}

bool TankAlgorithmImp::canShoot(int height, int width, const std::pair<int, int>& my_location, const std::pair<int, int>& enemy_location, const CanonDirection& my_direction, const std::vector<std::pair<int, int>>& wall_locations) const {
    return clearPathFromSrcToDst(height, width, my_location.first, my_location.second, enemy_location.first, enemy_location.second, my_direction, wall_locations);
}

bool TankAlgorithmImp::canMove(int new_x, int new_y,
    const std::vector<std::pair<int, int>>& wall_locations, const std::vector<std::pair<int, int>>& mine_locations, 
    const std::vector<std::pair<int, int>>& tanks1_locations ,const std::vector<std::pair<int, int>>& tanks2_locations) const
    {
        // Used chatGpt to learn how to combine vectors, prompt was "How to combine 4 vectors in c++ in the most efficient way"
        std::vector<std::pair<int, int>> bad_moves_locations;
        bad_moves_locations.reserve(wall_locations.size() + mine_locations.size() + tanks1_locations.size() + tanks2_locations.size());
        bad_moves_locations.insert(bad_moves_locations.end(), wall_locations.begin(), wall_locations.end());
        bad_moves_locations.insert(bad_moves_locations.end(), mine_locations.begin(), mine_locations.end());    
        bad_moves_locations.insert(bad_moves_locations.end(), tanks1_locations.begin(), tanks1_locations.end());
        bad_moves_locations.insert(bad_moves_locations.end(), tanks2_locations.begin(), tanks2_locations.end());
        for(const auto& bad : bad_moves_locations) {
            if (new_x == bad.first && new_y == bad.second) {
                return false;
            }
        }
        return true;
    }

bool TankAlgorithmImp::clearPathFromSrcToDst(int height, int width, const int src_x, const int src_y, const int dst_x, const int dst_y, const CanonDirection dir, const std::vector<std::pair<int, int>>& bad_moves_locations) const{
    int cx = src_x;
    int cy = src_y;
    int ox = dst_x;
    int oy = dst_y;
    
    for (int steps = 0; steps < std::max(height, width); ++steps) {
        if (cx == ox && cy == oy) {
            return true;
        }
        for (const auto& bad : bad_moves_locations) {
            if (cx == bad.first && cy == bad.second) {
                return false;
            }
        }

        switch (dir) {
            case CanonDirection::UP:
                cy = (cy - 1 + height) % height;
                break;
            case CanonDirection::DOWN:
                cy = (cy + 1) % height;
                break;
            case CanonDirection::LEFT:
                cx = (cx - 1 + width) % width;
                break;
            case CanonDirection::RIGHT:
                cx = (cx + 1) % width;
                break;
            case CanonDirection::UP_RIGHT:
                cy = (cy - 1 + height) % height;
                cx = (cx + 1) % width;
                break;
            case CanonDirection::UP_LEFT:
                cy = (cy - 1 + height) % height;
                cx = (cx - 1 + width) % width;
                break;
            case CanonDirection::DOWN_RIGHT:
                cy = (cy + 1) % height;
                cx = (cx + 1) % width;
                break;
            case CanonDirection::DOWN_LEFT:
                cy = (cy + 1) % height;
                cx = (cx - 1 + width) % width;
                break;
        }
    }
    return false; 
}


std::pair<int, int> TankAlgorithmImp::getClosestEnemyTank(const std::pair<int, int>& my_location, const std::vector<std::pair<int, int>>& enemy_tanks) const{
    // There shouldn't be a case were the return value is {-1,-1} because if the enemy doesn't have any live tanks the game will finish.
    int min_dist = std::numeric_limits<int>::max();
    std::pair<int, int> closest_enemy = {-1, -1};
    for (const auto& enemy_loc : enemy_tanks) {
        int dist = abs(my_location.first - enemy_loc.first) + abs(my_location.second - enemy_loc.second);
        if (dist < min_dist) {
            min_dist = dist;
            closest_enemy = enemy_loc;
        }
    }
    return closest_enemy;
}

std::pair<int, int> TankAlgorithmImp::getNextForwardLocation(const std::pair<int, int>& current_location, const CanonDirection& dir) const {
    int new_x = current_location.first;
    int new_y = current_location.second;

    switch(dir){
        case CanonDirection::UP: new_y--; break;
        case CanonDirection::DOWN: new_y++; break;
        case CanonDirection::LEFT: new_x--; break;
        case CanonDirection::RIGHT: new_x++; break;
        case CanonDirection::UP_RIGHT: new_y--; new_x++; break;
        case CanonDirection::UP_LEFT: new_y--; new_x--; break;
        case CanonDirection::DOWN_LEFT: new_y++; new_x--; break;
        case CanonDirection::DOWN_RIGHT: new_y++; new_x++; break;
        default: break;
    }
    new_x = (new_x + width) % width;
    new_y = (new_y + height) % height;
    return std::make_pair(new_x, new_y);
}

void TankAlgorithmImp::updateLocalState(ActionRequest action) {
    switch (action) 
    {
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

        case ActionRequest::Shoot:
            last_shoot_turn = turn_counter;
            break;

        default:
            break;    
    }
}

CanonDirection TankAlgorithmImp::rotate(CanonDirection cur_dir, int rotation) {
    // Used ChatGpt to cast the direction to number and preforme rotation as addition/substraction
    int new_dir = static_cast<int>(cur_dir) + rotation;
    if (new_dir < 0) {
        new_dir += 8; // Wrap around to the last direction
    } else if (new_dir >= 8) {
        new_dir -= 8; // Wrap around to the first direction
    }
    return static_cast<CanonDirection>(new_dir);
}