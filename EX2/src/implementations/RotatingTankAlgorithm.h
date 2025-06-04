#ifndef ROTATINGTANKALGORITHM_H
#define ROTATINGTANKALGORITHM_H
#include "../common/TankAlgorithm.h"
#include "../common/ActionRequest.h"

class RotatingTankAlgorithm : public TankAlgorithm {
    public:
        RotatingTankAlgorithm(int player_id, int tank_index);
        virtual ActionRequest getAction() override;
        virtual void updateBattleInfo(BattleInfo& info) override;
        virtual ~RotatingTankAlgorithm() override = default;
        int getPlayerId();
        int getTankIndex();

    private:
        int player_id;
        int tank_index;
        int turn_counter; // Counter to keep track of turns
};

#endif