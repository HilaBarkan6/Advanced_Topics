#include "SimpleTankAlgorithmFactory.h"
#include "SimpleTankAlgorithm.h"
#include <iostream>

// SimpleTankAlgorithmFactory::SimpleTankAlgorithmFactory() {}
// SimpleTankAlgorithmFactory::~SimpleTankAlgorithmFactory() {} 
unique_ptr<TankAlgorithm> SimpleTankAlgorithmFactory::create(int player_index, int tank_index) const {
    return std::make_unique<SimpleTankAlgorithm>(player_index, tank_index);
}