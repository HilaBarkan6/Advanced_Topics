#ifndef SIMPLE_PLAYER_H
#define SIMPLE_PLAYER_H
#include "../common/Player.h"
#include "../common/SatelliteView.h"
#include "../common/TankAlgorithm.h"
#include "SimpleBattleInfo.h"

class SimplePlayer : public Player {
    public:
        SimplePlayer(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells) : Player(player_index, x, y, max_steps, num_shells) {
            width = x;
            height = y;
        }
        ~SimplePlayer() override = default;
        virtual void updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& satellite_view) override;
    
    private:
        SimpleBattleInfo battle_info;
        int width;
        int height;

};


#endif
