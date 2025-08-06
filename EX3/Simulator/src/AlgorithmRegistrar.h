#ifndef ALGORITHM_REGISTRAR_H
#define ALGORITHM_REGISTRAR_H

#include <vector>
#include <string>

#include <memory>
#include <cassert>
#include <filesystem>
#include "../../common/Player.h"
#include "../../common/TankAlgorithm.h"
#include "SoOpener.h"

class AlgorithmRegistrar {
    class AlgorithmAndPlayerFactories {
        std::string so_name;
        TankAlgorithmFactory tankAlgorithmFactory;
        PlayerFactory playerFactory;

    public:
        AlgorithmAndPlayerFactories(const std::string& so_name) : so_name(so_name) {}
        void setTankAlgorithmFactory(TankAlgorithmFactory&& factory) {
            assert(tankAlgorithmFactory == nullptr);
            tankAlgorithmFactory = std::move(factory);
        }
        void setPlayerFactory(PlayerFactory&& factory) {
            assert(playerFactory == nullptr);
            playerFactory = std::move(factory);
        }
        const std::string& name() const { return so_name; }
        // std::string& name() { return so_name; }
        std::unique_ptr<Player> createPlayer(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells) const {
            return playerFactory(player_index, x, y, max_steps, num_shells);
        }
        std::unique_ptr<TankAlgorithm> createTankAlgorithm(int player_index, int tank_index) const {
            return tankAlgorithmFactory(player_index, tank_index);
        }
        bool hasPlayerFactory() const {
            return playerFactory != nullptr;
        }
        bool hasTankAlgorithmFactory() const {
            return tankAlgorithmFactory != nullptr;
        }
        TankAlgorithmFactory getTankAlgorithmFactory() const {
            return tankAlgorithmFactory;
        }
    };

    std::vector<AlgorithmAndPlayerFactories> algorithms; // Must be before algorithms_so to ensure correct destruction order
    std::vector<std::unique_ptr<SoOpener>> algorithms_so; // Must be below algorithms to release later
    static AlgorithmRegistrar registrar;

public:
    static AlgorithmRegistrar& getAlgorithmRegistrar();

    const std::vector<AlgorithmAndPlayerFactories>& getAlgorithms() const {
        return algorithms;
    }
    void openSo(const std::string& so_path) {
        algorithms.emplace_back(std::filesystem::path(so_path).stem().string());
        algorithms_so.emplace_back(std::make_unique<SoOpener>(so_path));
        try {
            validateLastRegistration();
        } catch (...) {
            removeLast();
            throw std::runtime_error("Failed to load algorithm: " + so_path);
        }
    }

    void createAlgorithmFactoryEntry(const std::string& name) {
        algorithms.emplace_back(name);
    }
    void addPlayerFactoryToLastEntry(PlayerFactory&& factory) {
        algorithms.back().setPlayerFactory(std::move(factory));
    }
    void addTankAlgorithmFactoryToLastEntry(TankAlgorithmFactory&& factory) {
        algorithms.back().setTankAlgorithmFactory(std::move(factory));
    }
    struct BadRegistrationException {
        std::string name;
        bool hasName, hasPlayerFactory, hasTankAlgorithmFactory;
    };
    void validateLastRegistration() {
        const auto& last = algorithms.back();
        bool hasName = (last.name() != "");
        if(!hasName || !last.hasPlayerFactory() || !last.hasTankAlgorithmFactory() ) {
            throw BadRegistrationException{
                .name = last.name(),
                .hasName = hasName,
                .hasPlayerFactory = last.hasPlayerFactory(),
                .hasTankAlgorithmFactory = last.hasTankAlgorithmFactory()
            };
        }
    }
    void removeLast() {
        algorithms.pop_back();
    }
    auto begin() const {
        return algorithms.begin();
    }
    auto end() const {
        return algorithms.end();
    }
    std::size_t count() const { return algorithms.size(); }
    void clear() { algorithms.clear(); }
};

#endif // ALGORITHM_REGISTRAR_H