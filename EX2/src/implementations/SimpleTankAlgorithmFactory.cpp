#include "SimpleTankAlgorithmFactory.h"
#include "SimpleTankAlgorithm.h"
#include "RotatingTankAlgorithm.h"
#include <iostream>

// SimpleTankAlgorithmFactory::SimpleTankAlgorithmFactory() {}
// SimpleTankAlgorithmFactory::~SimpleTankAlgorithmFactory() {} 
SimpleTankAlgorithmFactory::SimpleTankAlgorithmFactory(const Config& config)
    : battle_info_request_period(config.getInt("battle_info_request_period", DEFAULT_BATTLE_INFO_REQUEST_PERIOD)) {
}

unique_ptr<TankAlgorithm> SimpleTankAlgorithmFactory::create(int player_index, int tank_index) const {
    if(player_index == 1) {
        return std::make_unique<SimpleTankAlgorithm>(player_index, tank_index, battle_info_request_period);
    }
    
    return std::make_unique<RotatingTankAlgorithm>(player_index, tank_index);
    }