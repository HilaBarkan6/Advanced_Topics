#ifndef SIMPLETANKALGORITHM_H
#define SIMPLETANKALGORITHM_H
#include "../common/TankAlgorithm.h"
#include "../common/ActionRequest.h"
#include "../game_objects/CanonDirection.h"

class SimpleTankAlgorithm : public TankAlgorithm {
    public:
        SimpleTankAlgorithm(int player_id, int tank_index);
        ActionRequest getAction() override;
        void updateBattleInfo(BattleInfo& info) override;
        //~SimpleTankAlgorithm() override = default;
        int getPlayerId();
        int getTankIndex();
        CanonDirection rotate(CanonDirection cur_dir, int rotation);

    private:
        int player_id;
        int tank_index;
        int turn_counter;
        CanonDirection current_canon_direction;
        std::vector<ActionRequest> actions;


        
};


#endif