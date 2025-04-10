#include "Player.h"

void Player::setTank(Tank* tank) {
    this->tank = tank;
}

Tank* Player::getTank() const {
    return tank;
}