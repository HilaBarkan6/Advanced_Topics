#ifndef OFFENSIVE_PLAYER_H
#define OFFENSIVE_PLAYER_H
#include "Player.h"
#include <unordered_set>
#include <tuple>
#include <functional>
#include <utility>
#include <queue>



class OffensivePlayer : public Player {
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
        Player::Action firstAction;
        int depth;
    };

    public:
        OffensivePlayer(int id) : Player(id) {};
        virtual Action getAction(const Board& board, const Tank& tank1, const Tank& tank2, const bool allow_shoot) override;

    private:
        std::unordered_set<State, StateHash> visited;
        std::queue<QueueNode> q;
        Player::Action defensiveMove(const Board& board, const Tank& my_tank, const Tank& op_tank);
        Player::Action offensiveMove(const Board& board, const Tank& my_tank, const Tank& op_tank, const bool allow_shoot);
                
};


#endif