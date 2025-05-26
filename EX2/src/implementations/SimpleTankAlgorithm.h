#ifndef SIMPLETANKALGORITHM_H
#define SIMPLETANKALGORITHM_H
#include "../common/TankAlgorithm.h"
#include "../common/ActionRequest.h"
#include "../game_objects/CanonDirection.h"
#include "SimpleBattleInfo.h"
#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <unordered_set>

class SimpleTankAlgorithm : public TankAlgorithm {
    struct State{
        int x, y;
        CanonDirection dir;

        bool operator==(const State& other) const{
            return x == other.x && y == other.y && dir == other.dir;
        }
    };
        
    struct StateHash {
        std::size_t operator()(const State& state) const {
            return std::hash<int>()(state.x) ^ std::hash<int>()(state.y) ^ std::hash<int>()(static_cast<int>(state.dir));
        }
    };

    struct QueueNode{
        State state;
        ActionRequest firstAction;
        ActionRequest secondAction;
        int depth;
    };
    
    private:
        int player_id;
        int tank_index;
        int turn_counter;
        int height;
        int width;
        

        // Visited and q are for BFS algorithm, Suggested by ChatGpt
        std::unordered_set<State, StateHash> visited;
        std::queue<QueueNode> q;

        CanonDirection current_canon_direction;
        int last_shoot_turn;

        std::vector<ActionRequest> actions_to_apply;
        CanonDirection rotate(CanonDirection cur_dir, int rotation);
        std::pair<int, int> getClosestEnemyTank(const std::pair<int, int>& my_location, const std::vector<std::pair<int, int>>& enemy_tanks) const;
        bool canShoot(size_t height, size_t width, const std::pair<int, int>& my_location, const std::pair<int, int>& enemy_location, const CanonDirection& my_direction, const std::vector<std::pair<int, int>>& wall_locations) const;
        bool clearPathFromSrcToDst(size_t height, size_t width, const int src_x, const int src_y, const int dst_x, const int dst_y, const CanonDirection dir, const std::vector<std::pair<int, int>>&  bad_moves_locations) const;
        bool canMove(int new_x, int new_y, const std::vector<std::pair<int, int>>& wall_locations, const std::vector<std::pair<int, int>>& mine_locations, const std::vector<std::pair<int, int>>& tanks1_locations ,const std::vector<std::pair<int, int>>& tanks2_locations) const;
        void bfs(const std::pair<int, int>& enemy_location, const SimpleBattleInfo& simple_info);
        std::pair<int,int> getNextForwardLocation(const std::pair<int, int>& current_location, const CanonDirection& dir) const;
        
    
    public:
        SimpleTankAlgorithm(int player_id, int tank_index);
        ActionRequest getAction() override;
        void updateBattleInfo(BattleInfo& info) override;
    
};


#endif