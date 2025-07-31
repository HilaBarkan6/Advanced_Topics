#include "../../common/PlayerRegistration.h"
#include "AlgorithmRegistrar.h"
#include <iostream>

PlayerRegistration::PlayerRegistration(PlayerFactory factory) {
    auto& regsitrar = AlgorithmRegistrar::getAlgorithmRegistrar();
    regsitrar.addPlayerFactoryToLastEntry(std::move(factory));
}
