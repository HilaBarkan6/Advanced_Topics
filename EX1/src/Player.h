#ifndef PLAYER_H
#define PLAYER_H

#include "Board.h"
#include "game_objects/Tank.h"

class Player
{
    protected:
        int id;
   
    public:
        enum class Action{
            FORWARD,
            BACKWARD,
            ROTATE_025_LEFT,
            ROTATE_0125_RIGHT,
            ROTATE_025_RIGHT,
            ROTATE_0125_LEFT,
            SHOOT,
            NOTHING
        };
        Player(int id);
        virtual ~Player() = default;
        virtual Action getAction(const Board& board, const Tank& tank1, const Tank& tank2) = 0;
        int getId() const;

    
};

#endif