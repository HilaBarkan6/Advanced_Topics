#ifndef SIMPLE_PLAYER_H
#define SIMPLE_PLAYER_H
#include "PlayerImp.h"
#include "../../common/PlayerRegistration.h"
#include <map>
#include <set>

namespace Algorithm_209399021_208239152 {


class SimplePlayer : public PlayerImp {
    private:
        std::map<std::pair<int, int>, std::pair<int, int>> shells_movements;
        void analyzeShellsMovements(); // Analyze the projectile movements between two battle info states
        std::set<std::pair<int, int>> computeDangerPositions() const; // Compute the positions that are dangerous for the tank based on the shells_movements map

    public:
        SimplePlayer(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells)
            : PlayerImp(player_index, x, y, max_steps, num_shells) {}
        virtual ~SimplePlayer() = default;
        virtual void updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& satellite_view) override;
};

}

#endif