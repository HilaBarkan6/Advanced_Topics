#ifndef BFSTANKALGORITHM_H
#define BFSTANKALGORITHM_H
#include "TankAlgorithmImp.h"
#include "../common/ActionRequest.h"
#include "..UserCommon/configuration/Config.h"
#include <iostream>
#include <queue>
#include <unordered_set>

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
        static constexpr int BFS_MAX_DEPTH = 10; // Maximum depth for BFS, can be change in config.txt file

        int bfs_max_depth;
        // Visited and q are for BFS algorithm, Suggested by ChatGpt
        std::unordered_set<State, StateHash> visited;
        std::queue<QueueNode> q;

        std::vector<ActionRequest> actions_to_apply;
        //std::pair<int, int> getClosestEnemyTank(const std::pair<int, int>& my_location, const std::vector<std::pair<int, int>>& enemy_tanks) const;
        void bfs(const std::pair<int, int>& enemy_location, const SimpleBattleInfo& simple_info);
        
        
        // Helper functions for BFS
        bool tryShoot(const QueueNode& current, const SimpleBattleInfo& info, const std::pair<int, int>& enemy_location);
        void tryMoveForward(const QueueNode& current, const SimpleBattleInfo& info);
        void tryRotations(const QueueNode& current); // 

    public:
        BFSTankAlgorithm(int player_id, int tank_index, int battle_info_request_period, int shooting_waiting_turns);
        virtual ActionRequest getAction() override;
        virtual void updateBattleInfo(BattleInfo& info) override; 
        virtual ~BFSTankAlgorithm() override = default;
};

#endif