#include "ComparativeRunner.h"

namespace fs = std::filesystem;

ComparativeRunner::ComparativeRunner(const ParsedArguments& args) : args(args) {}

void ComparativeRunner::run() {
    GameInput input = tryReadMap(args.game_map); // Read the game map from the specified file
    tryLoadAlgorithms(args.algorithm1, args.algorithm2); // Load the algorithm shared libraries
    auto gm_paths = findGameManagers(args.game_managers_folder); // Find all GameManager .so files in the specified folder
    tryLoadGameManagers(gm_paths); // Load GameManager shared libraries
    auto result_map = runAllGames(gm_paths, input); // Run all games with the loaded GameManagers and algorithms

    writeResults(result_map, input);
    AlgorithmRegistrar::getAlgorithmRegistrar().clear(); // Clear the algorithm registrar
    GameManagerRegistrar::getGameManagerRegistrar().clear(); // Clear the GameManager registrar
}

GameInput ComparativeRunner::tryReadMap(const std::string& path) {
    auto timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    std::string input_error_path = "input_error_" + std::to_string(timestamp) + ".txt";
    try {
        return readBoard(path, input_error_path);
    } catch (const std::exception& e) {
        throw std::runtime_error("Error reading map file: " + std::string(e.what()));
    }
}


void ComparativeRunner::tryLoadAlgorithms(const std::string& a1, const std::string& a2) {
    auto& registrar = AlgorithmRegistrar::getAlgorithmRegistrar();
    if (fs::equivalent(a1, a2)) {
        registrar.openSo(a1);
    } 
    else {
        registrar.openSo(a1);
        registrar.openSo(a2);   
    }
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


void ComparativeRunner::tryLoadGameManagers(const std::vector<std::string>& gm_paths){
    auto& gm_registrar = GameManagerRegistrar::getGameManagerRegistrar();
    for (const auto& path : gm_paths) {
        gm_registrar.openSo(path);
    }
}


SatelliteViewImp ComparativeRunner::createSatelliteView(const GameInput& input) const {
    SatelliteViewImp view;
    view.setSatelliteView(input.board);
    view.setRowsAndColumns(input.height, input.width);
    return view;
}


std::map<std::string, std::set<std::string>> ComparativeRunner::runAllGames(
    const std::vector<std::string>& gm_paths, const GameInput& input) {

    auto& algo_registrar = AlgorithmRegistrar::getAlgorithmRegistrar();
    //auto& gm_registrar = GameManagerRegistrar::getGameManagerRegistrar();

    std::map<std::string, std::set<std::string>> result_map;
    std::mutex result_mutex;

    bool same_algorithm = (args.algorithm1 == args.algorithm2) || fs::equivalent(args.algorithm1, args.algorithm2);
    if (algo_registrar.count() < 2 && !same_algorithm) {
        std::cerr << "Not enough algorithms registered (need 2).\n";
        return result_map;
    }

    int num_threads = args.num_threads > 0 ? args.num_threads : 1;
    if (num_threads <= 1 || gm_paths.size() <= 1) {
        for (size_t i = 0; i < gm_paths.size(); ++i) {
            runSingleGame(gm_paths[i], i, input, result_map);
        }
        return result_map;
    }

    std::vector<std::thread> workers;
    std::atomic<size_t> index{0};

    auto worker = [&]() {
        while (true) {
            size_t i = index.fetch_add(1);
            if (i >= gm_paths.size()) break;
            runSingleGame(gm_paths[i], i, input, result_map, &result_mutex);
        }
    };

    for (int i = 0; i < args.num_threads; ++i)
        workers.emplace_back(worker);

    // Main thread also works
    worker();

    for (auto& t : workers)
        t.join();

    return result_map;
}

void ComparativeRunner::runSingleGame(const std::string& path, int gm_index, const GameInput& input,
                                      std::map<std::string, std::set<std::string>>& result_map,
                                      std::mutex* result_mutex) {
    auto& algo_registrar = AlgorithmRegistrar::getAlgorithmRegistrar();
    auto& gm_registrar = GameManagerRegistrar::getGameManagerRegistrar();

    try {
        std::string gm_name = fs::path(path).stem().string();
        auto gm_entry = (gm_registrar.get(gm_index));
        std::unique_ptr<AbstractGameManager> gm = gm_entry->create(args.verbose);

        const auto& a1 = algo_registrar.getAlgorithms()[0];
        const auto& a2 = (algo_registrar.count() > 1) ? algo_registrar.getAlgorithms()[1] : a1;

        auto player1 = a1.createPlayer(1, input.width, input.height, input.max_steps, input.num_shells);
        auto player2 = a2.createPlayer(2, input.width, input.height, input.max_steps, input.num_shells);

        auto view = createSatelliteView(input);
        auto result = gm->run(
            input.width, input.height, view, input.input_file_name,
            input.max_steps, input.num_shells,
            *player1, a1.name(), *player2, a2.name(),
            a1.getTankAlgorithmFactory(), a2.getTankAlgorithmFactory()
        );

        std::string key = formatResult(result, input.max_steps, input.width, input.height);

        // lock the mutex to safely update the result map by multiple threads.
        if (result_mutex) {
            std::lock_guard<std::mutex> lock(*result_mutex);
            result_map[key].insert(fs::path(path).filename().stem().string());
        } 
        else {
            result_map[key].insert(fs::path(path).filename().stem().string());
        }

    } catch (const std::exception& e) {
        std::cerr << "Error running game manager " << path << ": " << e.what() << std::endl;
    }
}


void ComparativeRunner::writeResults(
    const std::map<std::string, std::set<std::string>>& result_map, const GameInput& ) {

    std::ostringstream filename;
    auto timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    filename << args.game_managers_folder << "/comparative_results_" << timestamp << ".txt";
    std::ofstream out(filename.str());

    if (!out.is_open()) {
        std::cerr << "Error: Failed to create results file at \"" << filename.str() << "\". "
                  << "Outputting results to screen instead:\n\n";

                  
        std::cout << "game_map=" << std::filesystem::path(args.game_map).string() << "\n";
        std::cout << "algorithm1=" << std::filesystem::path(args.algorithm1).stem().string() << "\n";
        std::cout << "algorithm2=" << std::filesystem::path(args.algorithm2).stem().string() << "\n\n";

        for (const auto& [res, gms] : result_map) {
            std::cout << join(gms) << "\n" << res << "\n\n";
        }
        return;
    }

    out << "game_map=" << std::filesystem::path(args.game_map).string() << "\n";
    out << "algorithm1=" << std::filesystem::path(args.algorithm1).stem().string() << "\n";
    out << "algorithm2=" << std::filesystem::path(args.algorithm2).stem().string() << "\n\n";

    for (const auto& [res, gms] : result_map) {
        out << join(gms) << "\n" << res << "\n\n";
    }

    std::cout << "Results written to " << filename.str() << "\n";
}

std::string ComparativeRunner::formatResult(const GameResult& r, size_t max_steps, size_t width, size_t height) {
    std::ostringstream ss;

    //Tie
    if(r.winner == 0){
        if(r.reason == GameResult::ZERO_SHELLS) {
            ss << "Tie, both players have zero shells for 40 steps";
        } else if(r.reason == GameResult::MAX_STEPS) {
            ss << "Tie, reached max steps = " << max_steps << " , player 1 has " << r.remaining_tanks[0] << " tanks, player 2 has " << r.remaining_tanks[1] << " tanks";
        } else if(r.reason == GameResult::ALL_TANKS_DEAD) {
            ss << "Tie, both players have zero tanks";
        }
    }

    //Player 1 won
    if(r.winner == 1) {
        ss << "Player 1 won with " << r.remaining_tanks[0] << " tanks still alive";
    }
    if(r.winner == 2) {
        ss << "Player 2 won with " << r.remaining_tanks[1] << " tanks still alive";
    }
    ss << std::endl;
    ss << r.rounds;
    ss << std::endl;
    for(size_t y = 0; y < height; y++) {
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
        if (it != items.begin()) ss << ", ";
        ss << *it;
    }
    return ss.str();
}