#ifndef Player_209399021_208239152_H
#define Player_209399021_208239152_H
#include "PlayerImp.h"
#include "../../common/PlayerRegistration.h"
#include <map>
#include <set>

namespace Algorithm_209399021_208239152 {
    class Player_209399021_208239152 : public PlayerImp {  
        public:
            Player_209399021_208239152(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells)
                : PlayerImp(player_index, x, y, max_steps, num_shells) {}
            virtual ~Player_209399021_208239152() = default;
            virtual void updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& satellite_view) override;
    };
}

#endif