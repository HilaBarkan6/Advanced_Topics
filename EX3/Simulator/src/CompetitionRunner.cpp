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

    void* gm_handle = dlopen(args.game_manager.c_str(), RTLD_LAZY);
    if (!gm_handle) {
        std::cerr << "Failed to load GameManager .so file.\n";
        return;
    }

    std::map<std::string, int> score_table;
    std::vector<void*> algo_handles = loadAllAlgorithmHandles(algorithm_paths, score_table);
    runAllGames(maps, algorithm_paths, score_table);
    writeResults(score_table);
}

std::vector<void*> CompetitionRunner::loadAllAlgorithmHandles(
    const std::vector<std::string>& algorithm_paths,
    std::map<std::string, int>& score_table) {

    std::vector<void*> handles;
    for (const auto& path : algorithm_paths) {
        void* handle = dlopen(path.c_str(), RTLD_LAZY);
        if (!handle) {
            std::cerr << "Failed to load algorithm: " << path << "\n";
            continue;
        }
        handles.push_back(handle);
        score_table[fs::path(path).filename().string()] = 0;
    }
    return handles;
}

void CompetitionRunner::runAllGames(const std::vector<GameInput>& maps,
                                    const std::vector<std::string>& algorithm_paths,
                                    std::map<std::string, int>& score_table) {

    auto algo_factories = getAllTankAlgorithmFactories();
    auto player_factories = getAllPlayerFactories();
    auto gm_factories = getAllGameManagerFactories();

    if (algo_factories.size() < 2 || player_factories.size() < 2 || gm_factories.empty()) {
        std::cerr << "Missing factories for GameManager or Algorithms.\n";
        return;
    }

    for (size_t k = 0; k < maps.size(); ++k) {
        auto pairs = generatePairs(k, algorithm_paths.size());
        for (const auto& [i, j] : pairs) {
            runSingleGameAndScore(maps[k], i, j, gm_factories, player_factories, algo_factories, algorithm_paths, score_table);
        }
    }
}

void CompetitionRunner::runSingleGameAndScore(const GameInput& map, int i, int j,
    const std::vector<GameManagerFactory>& gm_factories,
    const std::vector<PlayerFactory>& player_factories,
    const std::vector<TankAlgorithmFactory>& algo_factories,
    const std::vector<std::string>& algo_paths,
    std::map<std::string, int>& score_table) {

    auto gm = gm_factories[0](args.verbose);
    auto p1 = player_factories[0](1, map.width, map.height, map.max_steps, map.num_shells);
    auto p2 = player_factories[1](2, map.width, map.height, map.max_steps, map.num_shells);

    GameResult r = gm->run(
        map.width, map.height, map.board,
        map.max_steps, map.num_shells,
        *p1, *p2,
        algo_factories[i], algo_factories[j]
    );

    auto a1_name = fs::path(algo_paths[i]).filename().string();
    auto a2_name = fs::path(algo_paths[j]).filename().string();

    if (r.winner == 1)
        score_table[a1_name] += 3;
    else if (r.winner == 2)
        score_table[a2_name] += 3;
    else {
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