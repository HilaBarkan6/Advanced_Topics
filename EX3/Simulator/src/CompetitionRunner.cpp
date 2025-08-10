#include "CompetitionRunner.h"

namespace fs = std::filesystem;

CompetitionRunner::CompetitionRunner(const ParsedArguments& args) : args(args) {}

void CompetitionRunner::run() {
    auto maps = loadAllMaps();
    if (maps.empty()) {
        std::cerr << "Competition requires at least one map.\n";
        printUsage();
        return;
    }

    auto algorithm_paths = findAlgorithmPaths();
    int N = algorithm_paths.size();
    if (N < 2) {
        std::cerr << "Competition requires at least 2 algorithms.\n";
        printUsage();
        return;
    }

    auto& gm_registrar = GameManagerRegistrar::getGameManagerRegistrar();
    gm_registrar.openSo(args.game_manager); 

    std::map<std::string, int> score_table;
    loadAllAlgorithmHandles(algorithm_paths, score_table);
    runAllGames(maps, algorithm_paths, score_table);
    writeResults(score_table);

    AlgorithmRegistrar::getAlgorithmRegistrar().clear();
    GameManagerRegistrar::getGameManagerRegistrar().clear();
}


void CompetitionRunner::loadAllAlgorithmHandles( const std::vector<std::string>& algorithm_paths, std::map<std::string, int>& score_table) {

    auto& registrar = AlgorithmRegistrar::getAlgorithmRegistrar();

    for (const auto& path : algorithm_paths) {
        registrar.openSo(path); // Open the shared object file
        std::string name = fs::path(path).stem().string(); // Get filename without extension
        score_table[name] = 0;
    }

    if (score_table.size() < 2){
        throw std::runtime_error("At least two valid algorithms are required.");
    }
}


void CompetitionRunner::runAllGames(const std::vector<GameInput>& maps, const std::vector<std::string>& algorithm_paths, std::map<std::string, int>& score_table) {
    auto& algo_registrar = AlgorithmRegistrar::getAlgorithmRegistrar();
    auto& gm_registrar = GameManagerRegistrar::getGameManagerRegistrar();

    if (algo_registrar.count() < 2) {
        std::cerr << "Error: Not enough algorithms registered.\n";
        return;
    }
    if (gm_registrar.count() < 1) {
        std::cerr << "Error: No GameManager registered.\n";
        return;
    }

    struct GameTask {
        int map_index;
        int i;
        int j;
    };
    std::vector<GameTask> tasks;
    for (size_t k = 0; k < maps.size(); ++k) {
        auto pairs = generatePairs(k, algo_registrar.count());
        for (auto& p : pairs) {
            tasks.push_back({static_cast<int>(k), p.first, p.second});
        }
    }

    int num_threads = args.num_threads > 0 ? args.num_threads : 1;

    if (num_threads <= 1 || tasks.size() <= 1) {
        for (const auto& task : tasks) {
            runSingleGame(maps[task.map_index], task.i, task.j, score_table, algorithm_paths);
        }
        return;
    }

    std::mutex score_mutex;
    std::atomic<size_t> task_index{0};

    auto worker = [&]() {
        while (true) {
            size_t idx = task_index.fetch_add(1);
            if (idx >= tasks.size())
                break;
            const auto& t = tasks[idx];
            runSingleGame(maps[t.map_index], t.i, t.j, score_table, algorithm_paths, &score_mutex);
        }
    };

    std::vector<std::thread> workers;
    for (int t = 0; t < num_threads - 1; ++t)
        workers.emplace_back(worker);

    worker();

    for (auto& w : workers)
        w.join();
}

void CompetitionRunner::runSingleGame(const GameInput& map, int i, int j, std::map<std::string, int>& score_table, const std::vector<std::string>& algo_paths, std::mutex* score_mutex) {
    try{
        auto& algo_registrar = AlgorithmRegistrar::getAlgorithmRegistrar();
        auto& gm_registrar = GameManagerRegistrar::getGameManagerRegistrar();
        const auto& algorithms = algo_registrar.getAlgorithms();

        auto p1 = algorithms[i].createPlayer(1, map.width, map.height, map.max_steps, map.num_shells);
        auto p2 = algorithms[j].createPlayer(2, map.width, map.height, map.max_steps, map.num_shells);

        auto gm = gm_registrar.begin()->create(args.verbose); // יצירת מופע חדש של GameManager לכל משחק
        auto view = createSatelliteView(map);

        GameResult result = gm->run(
            map.width, map.height, view, map.input_file_name,
            map.max_steps, map.num_shells,
            *p1, algorithms[i].name(), *p2, algorithms[j].name(),
            algorithms[i].getTankAlgorithmFactory(),
            algorithms[j].getTankAlgorithmFactory()
        );

        auto a1 = fs::path(algo_paths[i]).stem().string();
        auto a2 = fs::path(algo_paths[j]).stem().string();

        if (score_mutex) {
            std::lock_guard<std::mutex> lock(*score_mutex);
            updateScore(score_table, result.winner, a1, a2);
        } else {
            updateScore(score_table, result.winner, a1, a2);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error running game between " << algo_paths[i] << " and " << algo_paths[j] 
                  << " on map " << map.input_file_name << ": " << e.what() << std::endl;
    }
}

SatelliteViewImp CompetitionRunner::createSatelliteView(const GameInput& map) const {
    SatelliteViewImp view;
    view.setSatelliteView(map.board);
    view.setRowsAndColumns(map.height, map.width);
    return view;
}

void CompetitionRunner::updateScore(std::map<std::string, int>& table, int winner, const std::string& a1, const std::string& a2) const {
    if (winner == 1) {
        table[a1] += 3;
    } else if (winner == 2) {
        table[a2] += 3;
    } else {
        table[a1] += 1;
        table[a2] += 1;
    }
}


std::vector<GameInput> CompetitionRunner::loadAllMaps() {
    std::vector<GameInput> maps;
    auto timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    std::string input_error_path = "input_error_" + std::to_string(timestamp) + ".txt";
    for (const auto& entry : fs::directory_iterator(args.game_maps_folder)) {
        //if (entry.path().extension() == ".txt") {
        try {
            maps.push_back(readBoard(entry.path().string(), input_error_path));
        } catch (...) {
            std::cerr << "Failed to read map: " << entry.path() << "\n";
        }
        //}
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
    auto timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    filename << args.algorithms_folder << "/competition_" << timestamp << ".txt";

    std::ofstream out(filename.str());
    if (!out.is_open()) {
        std::cerr << "Failed to write output file. Showing results below:\n";
        std::cout << "game_maps_folder=" << args.game_maps_folder << "\n";
        std::cout << "game_manager=" << std::filesystem::path(args.game_manager).stem().string() << "\n\n";

        auto sorted = sortScores(score_table);
        for (const auto& [name, score] : sorted)
            std::cout << name << " " << score << "\n";
    }

    out << "game_maps_folder=" << args.game_maps_folder << "\n";
    out << "game_manager=" << std::filesystem::path(args.game_manager).stem().string() << "\n\n";

    auto sorted = sortScores(score_table);
    for (const auto& [name, score] : sorted)
        out << name << " " << score << "\n";

    std::cout << "Results written to: " << filename.str() << "\n";
}

std::vector<std::pair<std::string, int>> CompetitionRunner::sortScores(const std::map<std::string, int>& scores) {
    std::vector<std::pair<std::string, int>> sorted(scores.begin(), scores.end());
    std::sort(sorted.begin(), sorted.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    return sorted;
}

void CompetitionRunner::printUsage() {
    std::cerr << "Usage: simulator <mode> <path_to_algorithms> <path_to_maps> <game_manager.so>\n"
              << " - mode: 'competition' or 'comparative'\n"
              << " - path_to_algorithms: folder containing algorithm .so files (at least 2 required)\n"
              << " - path_to_maps: folder containing game maps (at least 1 required)\n"
              << " - game_manager.so: shared library for the GameManager\n"
              << "Please ensure the inputs meet these requirements before running the competition.\n";
}