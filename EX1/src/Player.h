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
        virtual Action getAction(const Board& board, const Tank& my_tank, const Tank& op_tank, const bool allow_shoot) = 0;
        int getId() const;
        int shellIsComming(const Board& board, const Tank& my_tank, const Tank& op_tank, int x_to_check, int y_to_check) const;
        bool clearPath(const Board& board, int sx, int sy, int tx, int ty) const;
        bool canShootFromLocation(const Board& board, const Tank& op_tank, const int x, const int y, const CanonDirection dir) const;
        bool canMove(const Board& board, int x, int y, const Tank& op_tank, const Tank& my_tank) const;



    
};

#endif