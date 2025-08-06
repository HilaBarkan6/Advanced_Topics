#ifndef GAME_MANAGER_REGISTRAR_H
#define GAME_MANAGER_REGISTRAR_H

#include <memory>
#include <vector>
#include <functional>
#include <string>
#include <cassert>
#include "../../common/AbstractGameManager.h"
#include "SoOpener.h"

// Handles registration of dynamically loaded GameManager classes
class GameManagerRegistrar {

    struct GameManagerEntry {
        std::string so_name;
        GameManagerFactory factory;
        GameManagerEntry(const std::string& name) : so_name(name) {}
        void setFactory(GameManagerFactory&& f) {
            assert(factory == nullptr);  // Ensure only one factory per entry
            factory = std::move(f);
        }
        const std::string& name() const { return so_name; }
        std::unique_ptr<AbstractGameManager> create(bool verbose) const {
            return factory(verbose);
        }
        bool hasFactory() const { return factory != nullptr; }
        ~GameManagerEntry(){} // TODO: Do we need to implement rule of five here? 
    };
    
    std::vector<GameManagerEntry> managers;
    std::vector<std::unique_ptr<SoOpener>> gm_handles; // Should always be below managers to release later
    static GameManagerRegistrar registrar;

public:
    static GameManagerRegistrar& getGameManagerRegistrar();

    void openSo(const std::string& so_path);
    void createEntry(const std::string& name);
    void setFactoryToLast(GameManagerFactory&& factory);
    void validateLast();
    void removeLast();
    void clear();
    size_t count() const;

    auto begin() const { return managers.begin(); }
    auto end() const { return managers.end(); }

    struct BadRegistrationException {
        std::string name;
        bool hasName;
        bool hasFactory;
    };
};

#endif // GAME_MANAGER_REGISTRAR_H