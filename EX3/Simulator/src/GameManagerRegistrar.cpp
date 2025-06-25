#include "GameManagerRegistrar.h"

GameManagerRegistrar GameManagerRegistrar::registrar;

GameManagerRegistrar& GameManagerRegistrar::getGameManagerRegistrar() {
    return registrar;
}

void GameManagerRegistrar::createEntry(const std::string& name) {
    managers.emplace_back(name);
}

void GameManagerRegistrar::setFactoryToLast(GameManagerFactory&& factory) {
    managers.back().setFactory(std::move(factory));
}

void GameManagerRegistrar::validateLast() {
    const auto& last = managers.back();
    bool hasName = !last.name().empty();
    if (!hasName || !last.hasFactory()) {
        throw BadRegistrationException{
            .name = last.name(),
            .hasName = hasName,
            .hasFactory = last.hasFactory()
        };
    }
}

void GameManagerRegistrar::removeLast() {
    managers.pop_back();
}

void GameManagerRegistrar::clear() {
    managers.clear();
}

size_t GameManagerRegistrar::count() const {
    return managers.size();
}