#ifndef SECOND_PLAYER_H
#define SECOND_PLAYER_H
#include "../../src/Player.h"

class SecondPlayer : public Player {
    public:
        SecondPlayer() : Player() {};
        virtual Action getAction(const Board& board, const Tank& tank1, const Tank& tank2, const bool allow_shoot) override;        
};


#endif
