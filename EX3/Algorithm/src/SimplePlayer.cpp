
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
    analyzeShellsMovements();
    
    // Update the tank algorithm with the populated battle info 
    tank.updateBattleInfo(battle_info);
}

void SimplePlayer::analyzeShellsMovements() {
    shells_movements.clear(); // Clear previous movements map
    
    // Get shell positions at previous and current turns
    const auto& prev_shells = prev_battle_info.getShellsLocations();
    const auto& curr_shells = battle_info.getShellsLocations();

    // Track which previous shells got matched to current shells
    std::set<std::pair<int,int>> matched_prev_shells;

    // For each shell in the current state, try to find matching shell in previous state to infer movement vector
    for (const auto& curr_shell : curr_shells) {
        bool matched = false;
        for (const auto& prev_shell : prev_shells) {
            int dx = curr_shell.first - prev_shell.first;
            int dy = curr_shell.second - prev_shell.second;

            // We expect shell to move by at most 1 cell in any direction (including diagonals)
            if (std::abs(dx) <= 1 && std::abs(dy) <= 1) { // need to validate the movement with the counter not have to be 1
                // Found a matching projectile, record its movement vector
                shells_movements[curr_shell] = {dx, dy};
                matched = true;
                matched_prev_shells.insert(prev_shell);
                break;
            }
        }
        if (!matched) {
            // New shell — no previous position found
            shells_movements[curr_shell] = {0, 0}; // zero vector means "new shell" or unknown movement
        }
    }

    // We need to consider shells that exist in curr battle info but not in prev so we can remove the following for-loop
    // Mark shells that disappeared (not found in current) with special vector
    for (const auto& prev_shell : prev_shells) {
        if (matched_prev_shells.find(prev_shell) == matched_prev_shells.end()) {
            shells_movements[prev_shell] = {-999, -999};  // Disappeared shell (exploded)
        }
    }
}

std::set<std::pair<int, int>> BFSTankAlgorithm::computeDangerPositions() const {
    std::set<std::pair<int, int>> danger_positions;

    for (const auto& [pos, vec] : shells_movements) {
        if (vec.first == 0 && vec.second == 0) continue;        // new shell
        if (vec.first == -999 || vec.second == -999) continue;  // disappeared shell

        int x = pos.first;
        int y = pos.second;
        int dx = vec.first;
        int dy = vec.second;

        danger_positions.insert({x + dx, y + dy});
        danger_positions.insert({x + 2 * dx, y + 2 * dy});
    }

    return danger_positions;
}

}