#ifndef TANKALGORITHMIMP
#define TANKALGORITHMIMP

#include "../common/TankAlgorithm.h"
#include "../game_objects/CanonDirection.h"
#include <utility>
#include <vector>
#include <limits>
#include <cstdlib>


class TankAlgorithmImp : public TankAlgorithm {
    protected:
        int player_id;
        int tank_index;
        int turn_counter;
        int height;
        int width;
        int battle_info_request_period;

        CanonDirection current_canon_direction;
        int last_shoot_turn;

        std::pair<int, int> getClosestEnemyTank(const std::pair<int, int>& my_location, const std::vector<std::pair<int, int>>& enemy_tanks) const;
        bool canShoot(int height, int width, const std::pair<int, int>& my_location, const std::pair<int, int>& enemy_location, const CanonDirection& my_direction, const std::vector<std::pair<int, int>>& wall_locations) const;
        bool clearPathFromSrcToDst(int height, int width, const int src_x, const int src_y, const int dst_x, const int dst_y, const CanonDirection dir, const std::vector<std::pair<int, int>>&  bad_moves_locations) const;
        bool canMove(int new_x, int new_y, const std::vector<std::pair<int, int>>& wall_locations, const std::vector<std::pair<int, int>>& mine_locations, const std::vector<std::pair<int, int>>& tanks1_locations ,const std::vector<std::pair<int, int>>& tanks2_locations) const;
        std::pair<int,int> getNextForwardLocation(const std::pair<int, int>& current_location, const CanonDirection& dir) const;
        CanonDirection rotate(CanonDirection cur_dir, int rotation);


    public:
        TankAlgorithmImp(int player_id, int tank_index, int battle_info_request_period);

        virtual ActionRequest getAction() = 0;
        virtual void updateBattleInfo(BattleInfo& info) = 0;
        virtual ~TankAlgorithmImp() override = default;

        
};

#endif