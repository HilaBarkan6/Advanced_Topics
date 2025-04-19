#include "SimplePlayer.h"
#include <iostream>

Player::Action SimplePlayer::getAction(const Board& board, const Tank& my_tank, const Tank& op_tank, const bool allow_shoot){
    // Simple logic for the player action
    // For example, always move forward
    bool b = board.isEmptyLocation(my_tank.getLocationX(), op_tank.getLocationY() + 1);
    std::cout << "b is " << b << allow_shoot << std::endl;
    return Player::Action::FORWARD;
}

