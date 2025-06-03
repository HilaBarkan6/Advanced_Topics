#ifndef TANKALGORITHMIMP_H
#define TANKALGORITHMIMP_H
#include "../common/TankAlgorithm.h"
#include "../common/ActionRequest.h"
#include <vector>
#include <utility> // for std::pair

class TankAlgorithmImp : public TankAlgorithm {
    public:
        TankAlgorithmImp(int player_id, int tank_index);
        int getPlayerId();
        int getTankIndex();
        int shellIsComming(size_t width, size_t height, std::pair<int, int> location_to_check, std::vector<std::pair<int, int>> flying_shells_location); 

    private:
        int player_id;
        int tank_index;
        int turn_counter;
};

#endif