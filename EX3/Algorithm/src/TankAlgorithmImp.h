#ifndef TANKALGORITHMIMP
#define TANKALGORITHMIMP

#include "../../common/TankAlgorithm.h"
#include "../../UserCommon/CanonDirection.h"
#include "../../common/TankAlgorithmRegistration.h"
#include "../../UserCommon/configuration/Config.h"

#include "SimpleBattleInfo.h"

#include <utility>
#include <vector>
#include <limits>
#include <cstdlib>

namespace Algorithm_209399021_208239152 {

class TankAlgorithmImp : public TankAlgorithm {
    protected:
        int player_id;
        int tank_index;
        int turn_counter;
        // this will keep the last time we requested battle info, required to know how much distance shell moved between requests.
        int last_info_request_turn;
        int height;
        int width;
        int shooting_waiting_turns;
        CanonDirection current_canon_direction;
        int last_shoot_turn;

        std::pair<int, int> getClosestEnemyTank(const std::pair<int, int>& my_location, const std::vector<std::pair<int, int>>& enemy_tanks) const; // Returns the closest enemy tank location to my_location, or (-1, -1) if no enemy tanks are available.

        // Checks if the tank can shoot at the enemy tank located at enemy_location
        bool canShoot(int height, int width, const std::pair<int, int>& my_location, const std::pair<int, int>& enemy_location, const CanonDirection& my_direction, const std::vector<std::pair<int, int>>& wall_locations) const; 
        
        // Checks if there is a clear path from src to dst in the given direction, considering walls and bad moves locations.
        bool clearPathFromSrcToDst(int height, int width, const int src_x, const int src_y, const int dst_x, const int dst_y, const CanonDirection dir, const std::vector<std::pair<int, int>>&  bad_moves_locations) const;
        
        // Checks if the tank can move to the new location, considering walls, mines, and other tanks.
        bool canMove(int new_x, int new_y, const std::vector<std::pair<int, int>>& wall_locations, const std::vector<std::pair<int, int>>& mine_locations, const std::vector<std::pair<int, int>>& tanks1_locations ,const std::vector<std::pair<int, int>>& tanks2_locations) const;
        std::pair<int,int> getNextForwardLocation(const std::pair<int, int>& current_location, const CanonDirection& dir) const; // Returns the next location of the tank if it moves forward in the given direction.

        // If the action was rotate we update the current cannon direction. If the action was shoot we update the last shooting turn. On other actions no need to update anything.
        void updateLocalState(ActionRequest action);
        CanonDirection rotate(CanonDirection cur_dir, int rotation);


    public:
        TankAlgorithmImp(int player_id, int tank_index);

        virtual ActionRequest getAction() = 0;
        virtual void updateBattleInfo(BattleInfo& info) = 0;
        virtual ~TankAlgorithmImp() override = default;
};

}

#endif