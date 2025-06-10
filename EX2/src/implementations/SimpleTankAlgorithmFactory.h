#ifndef SIMPLE_TANK_ALGORITHM_FACTORY_H
#define SIMPLE_TANK_ALGORITHM_FACTORY_H   
#include "../common/TankAlgorithmFactory.h"
#include "../configuration/Config.h"
#include "BFSTankAlgorithm.h"
#include "SimpleTankAlgorithm.h"
#include "RotatingTankAlgorithm.h"
#include <iostream>

class SimpleTankAlgorithmFactory : public TankAlgorithmFactory {
    private:
        // Default values, can be configured in config.txt
        static constexpr int DEFAULT_BATTLE_INFO_REQUEST_PERIOD_BFS = 3; // Default period for requesting battle info, can be configured in config.txt
        static constexpr int DEFAULT_BATTLE_INFO_REQUEST_PERIOD_SIMPLE = 2;
        static constexpr int DEFAULT_SHOOTING_WAITING_TURNS = 4; 

        
        int battle_info_request_period_bfs;
        int battle_info_request_period_simple;
        int shooting_waiting_turns;
    
    public:
        virtual ~SimpleTankAlgorithmFactory() = default;
        SimpleTankAlgorithmFactory();
        virtual unique_ptr<TankAlgorithm> create(int player_index, int tank_index) const override;
};

#endif