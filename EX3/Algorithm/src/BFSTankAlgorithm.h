#ifndef BFSTANKALGORITHM_H
#define BFSTANKALGORITHM_H
#include "TankAlgorithmImp.h"
#include "../../common/ActionRequest.h"
#include "../UserCommon/configuration/Config.h"
#include <iostream>
#include <queue>
#include <unordered_set>
#include <map>
#include <set>

using namespace UserCommon_209399021_208239152;

namespace Algorithm_209399021_208239152 {
    class BFSTankAlgorithm : public TankAlgorithmImp {
        struct State{
            int x, y;
            CanonDirection dir;

            bool operator==(const State& other) const{
                return x == other.x && y == other.y && dir == other.dir;
            }
        };
            
        struct StateHash {
            std::size_t operator()(const State& state) const {
                return std::hash<int>()(state.x)<<1 ^ std::hash<int>()(state.y)<<2 ^ std::hash<int>()(static_cast<int>(state.dir));
            }
        };

        struct QueueNode{
            State state;
            ActionRequest firstAction;
            ActionRequest secondAction;
            int depth;
        };
        
        private:
            int bfs_max_depth;  //static constexpr int BFS_MAX_DEPTH = 10; // Maximum depth for BFS, can be change in config.txt file
            static constexpr int BATTLE_INFO_REQUEST_PERIOD = 3;  // This const cannot be changed therefore not in config file
            std::unordered_set<State, StateHash> visited;  // Visited and q are for BFS algorithm, Suggested by ChatGpt
            std::queue<QueueNode> q;
            std::vector<ActionRequest> actions_to_apply; // Actions to apply after BFS is done, in the order they were found
            //std::pair<int, int> getClosestEnemyTank(const std::pair<int, int>& my_location, const std::vector<std::pair<int, int>>& enemy_tanks) const;
            void bfs(const std::pair<int, int>& enemy_location, const SimpleBattleInfo& simple_info);
            
            // Helper functions for BFS
            bool tryShoot(const QueueNode& current, const SimpleBattleInfo& info, const std::pair<int, int>& enemy_location); // Try to shoot at the enemy tank if possible
            void tryMoveForward(const QueueNode& current, const SimpleBattleInfo& info, std::pair<std::set<std::pair<int, int>>, std::set<std::pair<int, int>>>& danger_positions); // Try to move forward if possible, considering danger positions
            void tryRotations(const QueueNode& current); // Try all rotation options and add them to the queue if not visited

            std::map<std::pair<int, int>, std::pair<int, int>> shells_movements; // Map to store the movements of the shells between two battle info states
            std::vector<std::pair<int, int>> prev_shells_locations; // Previous shells locations to compare with the current ones
            void analyzeShellsMovements(std::vector<std::pair<int, int>> cur_shells_locations); // Analyze the projectile movements between two battle info states
            std::pair<std::set<std::pair<int, int>>, std::set<std::pair<int, int>>> computeDangerPositions() const; // Compute the positions that are dangerous for the tank based on the shells_movements map

        public:
            BFSTankAlgorithm(int player_id, int tank_index); 
            virtual ActionRequest getAction() override; 
            virtual void updateBattleInfo(BattleInfo& info) override; 
            virtual ~BFSTankAlgorithm() override = default;
    };
}

#endif