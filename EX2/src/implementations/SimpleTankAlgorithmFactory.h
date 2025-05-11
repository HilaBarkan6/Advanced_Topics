#ifndef SIMPLE_TANK_ALGORITHM_FACTORY_H
#define SIMPLE_TANK_ALGORITHM_FACTORY_H   
#include "../common/TankAlgorithmFactory.h"

class SimpleTankAlgorithmFactory : public TankAlgorithmFactory {
    public:
        virtual unique_ptr<TankAlgorithm> create(int player_index, int tank_index) const override;
};

#endif