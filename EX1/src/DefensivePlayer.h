#ifndef DEFENSIVE_PLAYER_H
#define DEFENSIVE_PLAYER_H
#include "Player.h"

class DefensivePlayer : public Player {
    public:
        DefensivePlayer(int id) : Player(id) {};
        virtual Action getAction(const Board& board, const Tank& tank1, const Tank& tank2, const bool allow_shoot) override;
        bool canShoot(const Board& board, const Tank& my_tank, const Tank& op_tank) const;
        bool shellIsComming(const Board& board, const Tank& my_tank, const Tank& op_tank, int x_to_check, int y_to_check) const;
        bool clearPath(const Board& board, int sx, int sy, int tx, int ty) const;
        bool canMove(const Board& board, int x, int y, const Tank& op_tank, const Tank& my_tank) const;
          
};


#endif
