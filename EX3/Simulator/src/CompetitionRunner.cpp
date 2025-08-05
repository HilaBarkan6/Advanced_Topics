#include "CompetitionRunner.h"

namespace fs = std::filesystem;

CompetitionRunner::CompetitionRunner(const ParsedArguments& args) : args(args) {}

void CompetitionRunner::run() {
    auto maps = loadAllMaps();
    auto algorithm_paths = findAlgorithmPaths();
    int N = algorithm_paths.size();
    if (N < 2) {
        std::cerr << "Competition requires at least 2 algorithms.\n";
        return;
    }
    auto& gm_registrar = GameManagerRegistrar::getGameManagerRegistrar();
    gm_registrar.createEntry(args.game_manager);
    void* gm_handle = dlopen(args.game_manager.c_str(), RTLD_LAZY);
    if (!gm_handle) {
        std::cerr << "Failed to load GameManager .so file.\n";
        return;
    }

    std::map<std::string, int> score_table;
    std::vector<void*> algo_handles = loadAllAlgorithmHandles(algorithm_paths, score_table);
    runAllGames(maps, algorithm_paths, score_table);
    writeResults(score_table);
    // TODO - think if this should be here or in the destructor
    for (void* handle : algo_handles) {
        if(handle){
            dlclose(handle);
        } // Clean up loaded algorithm handles
    }
}

std::vector<void*> CompetitionRunner::loadAllAlgorithmHandles(
    const std::vector<std::string>& algorithm_paths,
    std::map<std::string, int>& score_table) {

    std::vector<void*> handles;
    auto& registrar = AlgorithmRegistrar::getAlgorithmRegistrar();

    for (const auto& path : algorithm_paths) {
        std::string name = fs::path(path).stem().string(); // Get filename without extension
        registrar.createAlgorithmFactoryEntry(fs::path(path).stem().string());

        void* handle = dlopen(path.c_str(), RTLD_LAZY); // Load .so file dynamically
        if (!handle) {
            const char* error_msg = dlerror();  // capture dlopen error
            std::cerr << "dlopen failed: " << (error_msg ? error_msg : "Unknown error") << std::endl;
            throw std::runtime_error("Failed to load algorithm .so: " + path);
        }

        try {
            registrar.validateLastRegistration(); // Check if both Player and TankAlgorithm are registered
            handles.push_back(handle);            // Store the handle for later cleanup
            score_table[name] = 0;                // Initialize score entry for this algorithm
        } catch (const AlgorithmRegistrar::BadRegistrationException& e) {
            std::cerr << "Bad registration in: " << name << "\n";
            registrar.removeLast();              // Remove failed registration entry
            dlclose(handle);                     // Close the library handle
        }
    }


    if (score_table.size() < 2)
        throw std::runtime_error("At least two valid algorithms are required.");

    return handles;
}

void CompetitionRunner::runAllGames(const std::vector<GameInput>& maps,
                                    const std::vector<std::string>& algorithm_paths,
                                    std::map<std::string, int>& score_table) {

    auto& algo_registrar = AlgorithmRegistrar::getAlgorithmRegistrar();
    auto& gm_registrar = GameManagerRegistrar::getGameManagerRegistrar();

    // Check that at least two algorithms are registered
    if (algo_registrar.count() < 2) {
        std::cerr << "Error: Not enough algorithms registered.\n";
        return;
    }
    // Check that at least one GameManager is registered
    if (gm_registrar.count() < 1) {
        std::cerr << "Error: No GameManager registered.\n";
        return;
    }

    // Loop over all maps
    for (size_t k = 0; k < maps.size(); ++k) {
        // Generate competing algorithm pairs for this map index
        auto pairs = generatePairs(k, algo_registrar.count());

        // Run games for each algorithm pair and each GameManager
        for (const auto& [i, j] : pairs) {
            runSingleGameAndScore(
                maps[k],        // Current map
                i, j,           // Algorithm indices
                score_table,
                algorithm_paths     // Score tracking
            );
        
        }
    }
}

void CompetitionRunner::runSingleGameAndScore(const GameInput& map, int i, int j,
    std::map<std::string, int>& score_table,
    const std::vector<std::string>& algo_paths) {

    auto& algo_registrar = AlgorithmRegistrar::getAlgorithmRegistrar();
    auto& gm_registrar = GameManagerRegistrar::getGameManagerRegistrar();

    const auto& algorithms = algo_registrar.getAlgorithms();
    // Create players using player factories from the algorithms registrar
    auto p1 = algorithms[i].createPlayer(1, map.width, map.height, map.max_steps, map.num_shells);
    auto p2 = algorithms[j].createPlayer(2, map.width, map.height, map.max_steps, map.num_shells);

    // Get tank algorithm factories for each algorithm
    auto tank_algo_factory1 = algorithms[i].getTankAlgorithmFactory();
    auto tank_algo_factory2 = algorithms[j].getTankAlgorithmFactory();

    auto game_manager = gm_registrar.begin()->create(args.verbose); // Create GameManager instance

    // Run the game with the single GameManager instance provided
    // TODO - currently the board is matrix of chars but is should be satellite view.

    SatelliteViewImp view;
    view.setSatelliteView(map.board);
    view.setRowsAndColumns(map.height, map.width);

    GameResult result = game_manager->run(
        map.width, map.height,
        view,
        "hello_map", // Placeholder for map name
        map.max_steps, map.num_shells,
        *p1, algorithms[i].name(), *p2, algorithms[j].name(),
        tank_algo_factory1,
        tank_algo_factory2
    );

    

    // Extract algorithm file names for score bookkeeping
    
    auto a1_name = fs::path(algo_paths[i]).stem().string(); //fs::path(algo_paths[i]).filename().string();
    auto a2_name = fs::path(algo_paths[j]).stem().string();// fs::path(algo_paths[j]).filename().string();

    // Update scores based on the game result
    if (result.winner == 1) {
        score_table[a1_name] += 3;
    } else if (result.winner == 2) {
        score_table[a2_name] += 3;
    } else {
        // Draw case: both get 1 point
        score_table[a1_name] += 1;
        score_table[a2_name] += 1;
    }
}

std::vector<GameInput> CompetitionRunner::loadAllMaps() {
    std::vector<GameInput> maps;
    for (const auto& entry : fs::directory_iterator(args.game_maps_folder)) {
        if (entry.path().extension() == ".txt") {
            try {
                maps.push_back(readBoard(entry.path().string()));
            } catch (...) {
                std::cerr << "Failed to read map: " << entry.path() << "\n";
            }
        }
    }
    if (maps.empty()) throw std::runtime_error("No valid maps found.");
    return maps;
}

std::vector<std::string> CompetitionRunner::findAlgorithmPaths() {
    std::vector<std::string> paths;
    for (const auto& entry : fs::directory_iterator(args.algorithms_folder)) {
        if (entry.path().extension() == ".so")
            paths.push_back(entry.path().string());
    }
    return paths;
}

std::vector<std::pair<int, int>> CompetitionRunner::generatePairs(int k, int N) {
    std::vector<std::pair<int, int>> pairs;
    for (int i = 0; i < N; ++i) {
        int j = (i + 1 + k % (N - 1)) % N;
        if (i < j)
            pairs.emplace_back(i, j);
    }
    return pairs;
}

void CompetitionRunner::writeResults(const std::map<std::string, int>& score_table) {
    std::ostringstream filename;
    filename << args.algorithms_folder << "/competition_" << std::time(nullptr) << ".txt";

    std::ofstream out(filename.str());
    if (!out.is_open()) {
        std::cerr << "Failed to write output file. Showing results below:\n";
        printResultsToStdout(score_table);
        return;
    }

    out << "game_maps_folder=" << args.game_maps_folder << "\n";
    out << "game_manager=" << args.game_manager << "\n\n";

    auto sorted = sortScores(score_table);
    for (const auto& [name, score] : sorted)
        out << name << " " << score << "\n";

    std::cout << "Results written to: " << filename.str() << "\n";
}

void CompetitionRunner::printResultsToStdout(const std::map<std::string, int>& score_table) {
    for (const auto& [name, score] : score_table)
        std::cout << name << " " << score << "\n";
}

std::vector<std::pair<std::string, int>> CompetitionRunner::sortScores(const std::map<std::string, int>& scores) {
    std::vector<std::pair<std::string, int>> sorted(scores.begin(), scores.end());
    std::sort(sorted.begin(), sorted.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    return sorted;
}