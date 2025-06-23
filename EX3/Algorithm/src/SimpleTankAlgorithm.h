#ifndef SIMPLETANKALGORITHM_H
#define SIMPLETANKALGORITHM_H

#include "TankAlgorithmImp.h"
#include "../common/ActionRequest.h"

class SimpleTankAlgorithm : public TankAlgorithmImp {
    private:
        ActionRequest action_to_apply;

    public:
        SimpleTankAlgorithm(int player_id, int tank_index, int battle_info_request_period, int shooting_waiting_turns)
            : TankAlgorithmImp(player_id, tank_index, battle_info_request_period, shooting_waiting_turns) {}

        virtual ActionRequest getAction() override;
        virtual void updateBattleInfo(BattleInfo& info) override;
        virtual ~SimpleTankAlgorithm() override = default;
};

#endif