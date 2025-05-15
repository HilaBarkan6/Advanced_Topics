#include "SimpleTankAlgorithmFactory.h"
#include "SimpleTankAlgorithm.h"
#include "RotatingTankAlgorithm.h"
#include <iostream>

// SimpleTankAlgorithmFactory::SimpleTankAlgorithmFactory() {}
// SimpleTankAlgorithmFactory::~SimpleTankAlgorithmFactory() {} 
unique_ptr<TankAlgorithm> SimpleTankAlgorithmFactory::create(int player_index, int tank_index) const {
    // if(player_index == 1){
    //     return std::make_unique<SimpleTankAlgorithm>(player_index, tank_index);
    // }
    // return std::make_unique<RotatingTankAlgorithm>(player_index, tank_index);
    return std::make_unique<SimpleTankAlgorithm>(player_index, tank_index);
}