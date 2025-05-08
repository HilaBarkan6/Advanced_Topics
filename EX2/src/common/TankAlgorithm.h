#ifdef TANKALGORITHM_H
#define TANKALGORITHM_H

#include "ActionRequest.h"

class TankAlgorithm {
    public:
        virtual ~TankAlgorithm() {}
        virtual ActionRequest getAction() = 0;
        virtual void updateBattleInfo(BattleInfo& info) = 0;
    };
    
#endif