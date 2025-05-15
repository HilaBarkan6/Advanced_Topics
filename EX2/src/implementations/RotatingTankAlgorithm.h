#ifndef ROTATINGTANKALGORITHM_H
#define ROTATINGTANKALGORITHM_H
#include "../common/TankAlgorithm.h"
#include "../common/ActionRequest.h"

class RotatingTankAlgorithm : public TankAlgorithm {
    public:
        RotatingTankAlgorithm(int player_id, int tank_index);
        ActionRequest getAction() override;
        void updateBattleInfo(BattleInfo& info) override;
        ~RotatingTankAlgorithm() override = default;
        int getPlayerId();
        int getTankIndex();

    private:
        int player_id;
        int tank_index;
};


#endif