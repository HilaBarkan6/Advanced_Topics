#ifndef SIMPLE_PLAYER_H
#define SIMPLE_PLAYER_H
#include "PlayerImp.h"


class SimplePlayer : public PlayerImp {
    public:
        explicit SimplePlayer(const Config& config, int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells)
            : PlayerImp(config, player_index, x, y, max_steps, num_shells)           
        {
        }
        virtual void updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& satellite_view) override;
};

#endif