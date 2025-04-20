#include "DefensivePlayer.h"
#include <iostream>

Player::Action DefensivePlayer::getAction(const Board& board, const Tank& my_tank, const Tank& op_tank, const bool allow_shoot){
    int height = board.getRows();
    int width = board.getColumns();
    int x = my_tank.getLocationX();
    int y = my_tank.getLocationY();
    int dx = 0;
    int dy = 0;
    CanonDirection dir = my_tank.getCanonDirection();

    if(allow_shoot && canShootFromLocation(board, op_tank, x, y, dir)){
        return Player::Action::SHOOT;
    }

    // if(shellIsComming(board, my_tank, op_tank, x, y)!=-1){
    switch(dir){
        case CanonDirection::UP: dx = -1; dy = 0; break;
        case CanonDirection::DOWN: dx = 1; dy = 0; break;
        case CanonDirection::LEFT: dx = 0; dy = -1; break;
        case CanonDirection::RIGHT: dx = 0; dy = 1; break;
        case CanonDirection::UP_RIGHT: dx = -1; dy = 1; break;
        case CanonDirection::UP_LEFT: dx = -1; dy = -1; break;
        case CanonDirection::DOWN_LEFT : dx = 1; dy = -1; break;
        case CanonDirection::DOWN_RIGHT: dx = 1; dy = 1; break;
        default: break;
    }

    int new_x_f = (x+dx + height) % height;
    int new_y_f = (y+dy + width) % width;
    int new_x_b = (x-dx + height) % height;
    int new_y_b = (y-dy + width) % width;

    
    if(canMove(board, new_x_f,  new_y_f, op_tank, my_tank)){
        return Player::Action::FORWARD;
    }
    else if(canMove(board, new_x_b, new_y_b, op_tank, my_tank)){
        return Player::Action::BACKWARD;
    }
    //else{
    return Player::Action::ROTATE_025_LEFT;
    //}
    
    // }
    // return Player::Action::FORWARD;

}

