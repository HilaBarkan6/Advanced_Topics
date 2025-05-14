#ifndef TANKALGORITHMFACTORY_H
#define TANKALGORITHMFACTORY_H

#include <memory>
#include "TankAlgorithm.h"

using std::unique_ptr;

class TankAlgorithmFactory {
    public:
        virtual ~TankAlgorithmFactory() {}
        virtual unique_ptr<TankAlgorithm> create(int player_index, int tank_index) const = 0;
    };
    
#endif