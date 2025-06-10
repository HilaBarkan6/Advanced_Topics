#include "SimpleTankAlgorithmFactory.h"

SimpleTankAlgorithmFactory::SimpleTankAlgorithmFactory()
    : battle_info_request_period_bfs(Config::getInstance().getInt("battle_info_request_period_bfs", DEFAULT_BATTLE_INFO_REQUEST_PERIOD_BFS)),
      battle_info_request_period_simple(Config::getInstance().getInt("battle_info_request_period_simple", DEFAULT_BATTLE_INFO_REQUEST_PERIOD_SIMPLE)),
      shooting_waiting_turns(Config::getInstance().getInt("shooting_waiting_turns", DEFAULT_SHOOTING_WAITING_TURNS)) {
}

unique_ptr<TankAlgorithm> SimpleTankAlgorithmFactory::create(int player_index, int tank_index) const {
    if(player_index == 1) {
        return std::make_unique<BFSTankAlgorithm>(player_index, tank_index, battle_info_request_period_bfs, shooting_waiting_turns);
    }
    return std::make_unique<SimpleTankAlgorithm>(player_index, tank_index, battle_info_request_period_simple, shooting_waiting_turns);

    // For tests only! 
    //return std::make_unique<RotatingTankAlgorithm>(player_index, tank_index);
}