#ifndef PLAYER_H
#define PLAYER_H

#include "Board.h"
#include "game_objects/Tank.h"

class Player
{
    protected:
        Tank* tank;

    public:
        enum class Action{
            FORWARD,
            BACKWARD,
            ROTATE_025_LEFT,
            ROTATE_0125_RIGHT,
            ROTATE_025_RIGHT,
            ROTATE_0125_LEFT,
            SHOOT
        };
        Player() : tank(nullptr) {};
        void setTank(Tank* tank);
        Tank* getTank() const;
        virtual ~Player() = default;
        virtual Action getAction(const Board& board, const Tank& tank1, const Tank& tank2) = 0;
};

#endif