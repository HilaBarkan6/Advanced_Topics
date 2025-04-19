#ifndef SECOND_PLAYER_H
#define SECOND_PLAYER_H
#include "Player.h"

class SecondPlayer : public Player {
    public:
        SecondPlayer(int id) : Player(id) {};
        virtual Action getAction(const Board& board, const Tank& tank1, const Tank& tank2, const bool allow_shoot) override;
    
        
};


#endif
