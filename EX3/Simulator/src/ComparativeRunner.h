#ifndef COMPARATIVE_RUNNER_H
#define COMPARATIVE_RUNNER_H

#include "CommandLineParser.h"
#include "BoardReader.h"
#include "AlgorithmRegistrar.h"
#include "GameManagerRegistrar.h"
#include "../../common/GameResult.h"
#include <string>
#include <vector>
#include <map>
#include <set>
#include "../../common/TankAlgorithmRegistration.h"
#include "../../common/GameManagerRegistration.h"
#include "../../UserCommon/SatelliteViewImp.h"
#include "../../UserCommon/configuration/config.h"

#include <filesystem>
#ifdef __unix__
#include <dlfcn.h>
#endif
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include <map>
#include <set>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <atomic>


class ComparativeRunner {
    private:
    ParsedArguments args; // Parsed command line arguments

    static constexpr std::string_view GAME_MANAGER_PRINT_NAME = "file_name";
    static constexpr std::string_view ALGORITHM_PRINT_NAME = "file_name";  
    static constexpr std::string_view MAP_PRINT_NAME = "full_path";

    std::string game_manager_print_name;
    std::string algorithm_print_name;
    std::string map_print_name;

    GameInput tryReadMap(const std::string& path);

    void tryLoadAlgorithms(const std::string& a1, const std::string& a2);

    // Scans the given folder for all .so files representing GameManagers. returns a vector of full paths to these files.
    std::vector<std::string> findGameManagers(const std::string& folder);

    void tryLoadGameManagers(const std::vector<std::string>& gm_paths);

    SatelliteViewImp createSatelliteView(const GameInput& input) const; // Creates a SatelliteViewImp object initialized with the game input data.
    std::map<std::string, std::set<std::string>> runAllGames(const std::vector<std::string>& gm_paths, const GameInput& input);

    void runSingleGame(const std::string& path, int gm_index, const GameInput& input,
                                      std::map<std::string, std::set<std::string>>& result_map,
                                      std::mutex* result_mutex = nullptr);

    // Writes the aggregated results to an output file in the GameManager folder.
    void writeResults(const std::map<std::string, std::set<std::string>>& result_map, const GameInput& input);

    // Converts a GameResult struct to a formatted string for output and comparison.
    std::string formatResult(const GameResult& r, size_t max_steps, size_t width, size_t height);

    // Joins a set of strings into a single comma-separated string.
    std::string join(const std::set<std::string>& items);
    
    public:
        explicit ComparativeRunner(const ParsedArguments& args);
        void run();
};

#endif