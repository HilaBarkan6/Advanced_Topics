#ifndef PLAYERIMP_H
#define PLAYERIMP_H
#include "../common/Player.h"
#include "../common/SatelliteView.h"
#include "../common/TankAlgorithm.h"
#include "SimpleBattleInfo.h"
#include <iostream>
#include <vector>
#include <utility> // for std::pair


class PlayerImp : public Player {
    public:
        PlayerImp(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells)
            : Player(player_index, x, y, max_steps, num_shells),
              battle_info(x, y),
              width(y),
              height(x)
        {
        }
        virtual void updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& satellite_view) =0;
    
    protected:
        SimpleBattleInfo battle_info;
        size_t width;
        size_t height;

        void putLocationByChar(char object_sign, int x, int y);
};


#endif
