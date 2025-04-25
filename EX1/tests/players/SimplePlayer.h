#ifndef SIMPLE_PLAYER_H
#define SIMPLE_PLAYER_H
#include "../../src/Player.h"

class SimplePlayer : public Player {
    public:
        SimplePlayer() : Player() {};
        virtual Action getAction(const Board& board, const Tank& tank1, const Tank& tank2, const bool allow_shoot) override;      
};


#endif
