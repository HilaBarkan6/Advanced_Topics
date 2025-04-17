#include "SecondPlayer.h"
#include <iostream>

Player::Action SecondPlayer::getAction(const Board& board, const Tank& tank1, const Tank& tank2){
    // Simple logic for the player action
    // For example, always move forward
    bool b = board.isEmptyLocation(tank1.getLocationX(), tank2.getLocationY() + 1);
    std::cout << "b is " << b << std::endl;
    return Player::Action::SHOOT;
}

