#ifndef SIMPLE_PLAYER_H
#define SIMPLE_PLAYER_H
#include "Player.h"

class SimplePlayer : public Player {
    public:
        SimplePlayer(int id) : Player(id) {};
        virtual Action getAction(const Board& board, const Tank& tank1, const Tank& tank2) override;
    
        
};


#endif
