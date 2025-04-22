#ifndef ROTATING_PLAYER_H
#define ROTATING_PLAYER_H
#include "Player.h"

class RotatingPlayer : public Player {
    public:
        RotatingPlayer() : Player() {};
        virtual Action getAction(const Board& board, const Tank& tank1, const Tank& tank2, const bool allow_shoot) override;      
};


#endif
