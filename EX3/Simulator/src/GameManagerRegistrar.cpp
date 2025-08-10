#include "GameManagerRegistrar.h"

GameManagerRegistrar GameManagerRegistrar::registrar;

GameManagerRegistrar& GameManagerRegistrar::getGameManagerRegistrar() {
    return registrar;
}

void GameManagerRegistrar::openSo(const std::string& so_path) {
    managers.emplace_back(so_path);
    gm_handles.emplace_back(std::make_unique<SoOpener>(so_path));
    try {
        validateLast();
    } catch (...) {
        removeLast();
        throw std::runtime_error("Failed to load GameManager: " + so_path);
    }
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

GameManagerRegistrar::GameManagerEntry* GameManagerRegistrar::get(int index) {
    if (index < 0 || index >= static_cast<int>(managers.size())) {
        throw std::out_of_range("Index out of range in GameManagerRegistrar");
    }
    return &managers[index];
}