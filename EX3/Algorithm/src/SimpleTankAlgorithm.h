#ifndef SIMPLETANKALGORITHM_H
#define SIMPLETANKALGORITHM_H

#include "TankAlgorithmImp.h"
#include "../../common/ActionRequest.h"

namespace Algorithm_209399021_208239152{

class SimpleTankAlgorithm : public TankAlgorithmImp {
    private:
        ActionRequest action_to_apply;

    public:
        SimpleTankAlgorithm(int player_id, int tank_index)
            : TankAlgorithmImp(player_id, tank_index) {}

        virtual ActionRequest getAction() override;
        virtual void updateBattleInfo(BattleInfo& info) override;
        virtual ~SimpleTankAlgorithm() override = default;
};

}

#endif