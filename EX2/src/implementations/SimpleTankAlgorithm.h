#ifndef SIMPLETANKALGORITHM_H
#define SIMPLETANKALGORITHM_H
#include "../common/TankAlgorithm.h"
#include "../common/ActionRequest.h"

class SimpleTankAlgorithm : public TankAlgorithm {
    public:
        SimpleTankAlgorithm(int player_id, int tank_index);
        ActionRequest getAction() override;
        void updateBattleInfo(BattleInfo& info) override;
        ~SimpleTankAlgorithm() override = default;
        int getPlayerId();
        int getTankIndex();

    private:
        int player_id;
        int tank_index;
};


#endif