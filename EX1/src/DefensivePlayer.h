#ifndef DEFENSIVE_PLAYER_H
#define DEFENSIVE_PLAYER_H
#include "Player.h"

class DefensivePlayer : public Player {
    public:
        DefensivePlayer(int id) : Player(id) {};
        virtual Action getAction(const Board& board, const Tank& tank1, const Tank& tank2, const bool allow_shoot) override;

          
};


#endif
