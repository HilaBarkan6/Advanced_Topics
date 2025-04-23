#ifndef PLAYER_H
#define PLAYER_H

#include "Board.h"
#include "game_objects/Tank.h"

class Player
{   
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

        Player(){};
        virtual ~Player() = default;

        virtual Action getAction(const Board& board, const Tank& my_tank, const Tank& op_tank, const bool allow_shoot) = 0;

        /* Given a location [x,y] to chech, returns the distance of the closest shell to this location
         * if it is far away, player might want to move to this location, if it is very close, player should not move to this location.
         * returns -1 if no shells are approching this location */
        int shellIsComming(const Board& board, const Tank& my_tank, const Tank& op_tank, int x_to_check, int y_to_check) const;

        // Given board, shell location, and tank location, check if the shell has a path without walls to the tank.
        bool clearPath(const Board& board, int sx, int sy, int tx, int ty) const;

        // Check if a tank in location [x,y] can shoot the op_tank with his current direction without hitting a wall.
        bool canShootFromLocation(const Board& board, const Tank& op_tank, const int x, const int y, const CanonDirection dir) const;

        // Check if tank can move to loction [x,y] without hitting wall or mine or the op_tank, 
        // and that there are no shells with distance from new location that is less then wanted_distance.
        bool canMove(const Board& board, int x, int y, const Tank& op_tank, const Tank& my_tank, int wanted_distance_from_shell) const;
};

#endif