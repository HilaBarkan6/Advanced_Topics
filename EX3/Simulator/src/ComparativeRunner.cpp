#include "ComparativeRunner.h"

namespace fs = std::filesystem;

ComparativeRunner::ComparativeRunner(const ParsedArguments& args) : args(args) {}

void ComparativeRunner::run() {
    GameInput input = tryReadMap(args.game_map); // Read the game map from the specified file
    algo_handles = tryLoadAlgorithms(args.algorithm1, args.algorithm2); // Load the algorithm shared libraries
    auto gm_paths = findGameManagers(args.game_managers_folder); // Find all GameManager .so files in the specified folder
    gm_handles = tryLoadGameManagers(gm_paths); // Load GameManager shared libraries
    auto result_map = runAllGames(gm_paths, input); // Run all games with the loaded GameManagers and algorithms

    writeResults(result_map, input);
    AlgorithmRegistrar::getAlgorithmRegistrar().clear(); // Clear the algorithm registrar
    GameManagerRegistrar::getGameManagerRegistrar().clear(); // Clear the GameManager registrar

    unloadSharedLibraries(algo_handles); // Clean up loaded algorithm handles
    unloadSharedLibraries(gm_handles); // Clean up loaded GameManager handles
}

GameInput ComparativeRunner::tryReadMap(const std::string& path) {
    try {
        return readBoard(path);
    } catch (const std::exception& e) {
        throw std::runtime_error("Error reading map file: " + std::string(e.what()));
    }
}

// std::vector<void*> ComparativeRunner::tryLoadAlgorithms(const std::string& a1, const std::string& a2) {
//     auto& registrar = AlgorithmRegistrar::getAlgorithmRegistrar();
//     std::vector<std::string> names = {fs::path(a1).stem().string(), fs::path(a2).stem().string()};
//     std::vector<void*> handles;

//     for (const auto& path : {a1, a2}) {
//         registrar.createAlgorithmFactoryEntry(fs::path(path).stem().string());
//         void* handle = dlopen(path.c_str(), RTLD_LAZY);
//         if (!handle){
//             const char* error_msg = dlerror();  // capture dlopen error
//             std::cerr << "dlopen failed: " << (error_msg ? error_msg : "Unknown error") << std::endl;
//             throw std::runtime_error("Failed to load algorithm .so: " + path);
//         }
//         handles.push_back(handle);
//         try {
//             registrar.validateLastRegistration();
//         } catch (...) {
//             registrar.removeLast();
//         }
//     }
//     return handles;
// }

std::vector<void*> ComparativeRunner::tryLoadAlgorithms(const std::string& a1, const std::string& a2) {
    auto& registrar = AlgorithmRegistrar::getAlgorithmRegistrar();
    std::vector<void*> handles;
    std::vector<std::string> algo_names;

    
    if (fs::equivalent(a1, a2)) {
       
        std::string base_name = fs::path(a1).stem().string();
        registrar.createAlgorithmFactoryEntry(base_name);
        void* handle = dlopen(a1.c_str(), RTLD_LAZY);
        if (!handle) throw std::runtime_error("Failed to load algorithm: " + a1);
        handles.push_back(handle);

        
        algo_names.push_back(base_name);
        algo_names.push_back(base_name);

    } else {
        
        for (const auto& path : {a1, a2}) {
            std::string name = fs::path(path).stem().string();
            registrar.createAlgorithmFactoryEntry(name);
            void* handle = dlopen(path.c_str(), RTLD_LAZY);
            if (!handle) throw std::runtime_error("Failed to load algorithm: " + path);
            handles.push_back(handle);
            algo_names.push_back(name);
        }
    }

   
    return handles;
}

std::vector<std::string> ComparativeRunner::findGameManagers(const std::string& folder) {
    std::vector<std::string> files;
    for (const auto& entry : fs::directory_iterator(folder)) {
        if (entry.path().extension() == ".so")
            files.push_back(entry.path().string());
    }
    if (files.empty())
        throw std::runtime_error("No GameManager .so files found in: " + folder);
    return files;
}

std::vector<void*> ComparativeRunner::tryLoadGameManagers(const std::vector<std::string>& gm_paths) {
    auto& gm_registrar = GameManagerRegistrar::getGameManagerRegistrar();
    std::vector<void*> handles;

    for (const auto& path : gm_paths) {
        gm_registrar.createEntry(path);  // Moved here, only once per path

        void* handle = dlopen(path.c_str(), RTLD_LAZY);
        if (!handle) {
            const char* error_msg = dlerror();
            throw std::runtime_error(std::string("Failed to load game manager: ") + path + "\n" + (error_msg ? error_msg : ""));
        }

        try {
            gm_registrar.validateLast();
        } 
        catch (...) {
            gm_registrar.removeLast();
            dlclose(handle);
            throw; 
        }

        handles.push_back(handle);
    }

    if (handles.empty()) {
        throw std::runtime_error("No valid GameManagers loaded");
    }

    return handles;
}


std::map<std::string, std::set<std::string>> ComparativeRunner::runAllGames(
    const std::vector<std::string>& gm_paths,
    const GameInput& input) {

    auto& algo_registrar = AlgorithmRegistrar::getAlgorithmRegistrar();
    auto& gm_registrar = GameManagerRegistrar::getGameManagerRegistrar();

    std::map<std::string, std::set<std::string>> result_map;

    bool same_algorithm = (args.algorithm1 == args.algorithm2) || fs::equivalent(args.algorithm1, args.algorithm2);

    if (algo_registrar.count() < 2 && !same_algorithm) {
        std::cerr << "Not enough algorithms registered (need 2).\n";
        return result_map;
    }

    for (size_t i = 0; i < gm_paths.size(); ++i) {
        const auto& path = gm_paths[i];
        
        try {
            const auto& gm_factory = *gm_registrar.begin();
            auto gm = gm_factory.create(args.verbose);

            const auto& a1 = algo_registrar.getAlgorithms()[0];
            const auto& a2 = (algo_registrar.count() > 1) ? algo_registrar.getAlgorithms()[1] : algo_registrar.getAlgorithms()[0];

            auto player1 = a1.createPlayer(1, input.width, input.height, input.max_steps, input.num_shells);
            auto player2 = a2.createPlayer(2, input.width, input.height, input.max_steps, input.num_shells);

            auto tank_factory1 = a1.getTankAlgorithmFactory();
            auto tank_factory2 = a2.getTankAlgorithmFactory();

            SatelliteViewImp view;
            view.setSatelliteView(input.board);
            view.setRowsAndColumns(input.height, input.width);

            GameResult result = gm->run(
                input.width, input.height,
                view,
                "hello_map",
                input.max_steps, input.num_shells,
                *player1, a1.name(), *player2, a2.name(),
                tank_factory1, tank_factory2
            );

            std::string key = formatResult(result, input.max_steps, input.width, input.height);
            result_map[key].insert(fs::path(path).filename().string());

        } catch (const std::exception& e) {
            std::cerr << "Error running game manager " << path << ": " << e.what() << std::endl;
            // Continue with next game manager without aborting all
            continue;
        }
    }

    return result_map;
}


// std::map<std::string, std::set<std::string>> ComparativeRunner::runAllGames(
//     const std::vector<std::string>& gm_paths, const GameInput& input) {

//     auto& algo_registrar = AlgorithmRegistrar::getAlgorithmRegistrar();
//     auto& gm_registrar = GameManagerRegistrar::getGameManagerRegistrar();

//     std::map<std::string, std::set<std::string>> result_map;
    
//     bool same_algorithm = (args.algorithm1 == args.algorithm2) || fs::equivalent(args.algorithm1, args.algorithm2);
//     if (algo_registrar.count() < 2 && !same_algorithm) {
//         std::cerr << "Not enough algorithms registered (need 2).\n";
//         return result_map;
//     }

//     for (const auto& path : gm_paths) {
    
//         gm_registrar.createEntry(path);
//         void* handle = dlopen(path.c_str(), RTLD_LAZY);
//         if (!handle) {
//             // CR: Consider throwing here as there is no need to continue running
//             const char* error_msg = dlerror();  // capture dlopen error
//             std::cerr << "Game manager dlopen failed: " << (error_msg ? error_msg : "Unknown error") << std::endl;
//             continue;
//         }

//         try {
//             gm_registrar.validateLast();
//         } catch (...) {
//             gm_registrar.removeLast();
//             continue;
//         }

//         const auto& gm_factory = *gm_registrar.begin();
//         auto gm = gm_factory.create(args.verbose);

//         const auto& a1 = algo_registrar.getAlgorithms()[0]; // index 0
//         const auto& a2 = (algo_registrar.count() > 1) ? algo_registrar.getAlgorithms()[1] : algo_registrar.getAlgorithms()[0]; 
                
//         auto player1 = a1.createPlayer(1, input.width, input.height, input.max_steps, input.num_shells);
//         auto player2 = a2.createPlayer(2, input.width, input.height, input.max_steps, input.num_shells);

//         auto tank_factory1 = a1.getTankAlgorithmFactory();
//         auto tank_factory2 = a2.getTankAlgorithmFactory();

//         SatelliteViewImp view;
//         view.setSatelliteView(input.board);
//         view.setRowsAndColumns(input.height, input.width);
        
//         GameResult result = gm->run(
//             input.width, input.height,
//             view,
//             "hello_map", // Placeholder for map name
//             input.max_steps, input.num_shells,
//             *player1, a1.name(), *player2, a2.name(),
//             tank_factory1, tank_factory2
//         );

//         std::string key = formatResult(result, input.max_steps, input.width, input.height);
//         result_map[key].insert(fs::path(path).filename().string());
//     }

//     return result_map;
// }

void ComparativeRunner::writeResults(
    const std::map<std::string, std::set<std::string>>& result_map, const GameInput& ) {

    std::ostringstream filename;
    filename << args.game_managers_folder << "/comparative_results_" << std::time(nullptr) << ".txt";
    std::ofstream out(filename.str());

    if (!out.is_open()) {
        std::cerr << "Failed to open result file. Outputting to screen:\n";
        for (const auto& [res, gms] : result_map)
            std::cout << join(gms) << "\n" << res << "\n\n";
        return;
    }

    out << "game_map=" << args.game_map << "\n";
    out << "algorithm1=" << args.algorithm1 << "\n";
    out << "algorithm2=" << args.algorithm2 << "\n\n";

    for (const auto& [res, gms] : result_map) {
        out << join(gms) << "\n" << res << "\n\n";
    }

    std::cout << "Results written to " << filename.str() << "\n";
}

std::string ComparativeRunner::formatResult(const GameResult& r, size_t max_steps, size_t width, size_t height) {
    std::ostringstream ss;

    //Tie
    if(r.winner == 0){
        if(r.reason == GameResult::ZERO_SHELLS){
            ss << "Tie, both players have zero shells for 40 steps";
        } else if(r.reason == GameResult::MAX_STEPS){
            ss << "Tie, reached max steps = " << max_steps << " , player 1 has " << r.remaining_tanks[0] << " tanks, player 2 has " << r.remaining_tanks[1] << " tanks";
        } else if(r.reason == GameResult::ALL_TANKS_DEAD){
            ss << "Tie, both players have zero tanks";
        }

    }
    //Player 1 won
    if(r.winner == 1){
        ss << "Player 1 won with " << r.remaining_tanks[0] << " tanks still alive";
    }
    if(r.winner == 2){
        ss << "Player 2 won with " << r.remaining_tanks[1] << " tanks still alive";
    }
    ss << std::endl;
    ss << r.rounds;
    ss << std::endl;
    for(size_t y = 0; y<height; y++){
        for(size_t x = 0; x < width; x++){
            char object = r.gameState->getObjectAt(x, y);
            ss << object;
        }
        ss << std::endl;
    }
    return ss.str();
}

std::string ComparativeRunner::join(const std::set<std::string>& items) {
    std::ostringstream ss;
    for (auto it = items.begin(); it != items.end(); ++it) {
        if (it != items.begin()) ss << ",";
        ss << *it;
    }
    return ss.str();
}

void ComparativeRunner::unloadSharedLibraries(const std::vector<void*>& handles) {
    for (void* handle : handles) {
        if (handle != nullptr) {
            dlclose(handle);
        }
    }
}
