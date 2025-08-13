#ifndef COMPETITION_RUNNER_H
#define COMPETITION_RUNNER_H

#include "BoardReader.h"
#include "CommandLineParser.h"
#include "AlgorithmRegistrar.h"
#include "GameManagerRegistrar.h"
#include "../../common/TankAlgorithmRegistration.h"
#include "../../common/GameManagerRegistration.h"
#include "../../common/PlayerRegistration.h"
#include "../../UserCommon/SatelliteViewImp.h"
#include "../../UserCommon/configuration/config.h"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <utility>

#include <filesystem>
#include <fstream>
#ifdef __unix__
#include <dlfcn.h>
#endif
#include <iostream>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <atomic>

class CompetitionRunner {

    private:
        struct GameTask {
            int map_index;
            int i;
            int j;
        };

        ParsedArguments args;

        static constexpr std::string_view GAME_MANAGER_PRINT_NAME = "file_name";
        static constexpr std::string_view MAPS_DIR_PRINT_NAME = "full_path";  
        static constexpr std::string_view ALGORITHM_PRINT_NAME = "file_name";  

        
        std::string game_manager_print_name;
        std::string maps_dir_print_name;
        std::string algorithm_print_name;
    

        // Loads all valid .map files from the game_maps_folder
        std::vector<GameInput> loadAllMaps();

        // Loads all .so files from algorithms_folder
        std::vector<std::string> findAlgorithmPaths();

        // Opens .so files and adds each algorithm to the score table
        void loadAllAlgorithmHandles(
            const std::vector<std::string>& algorithm_paths,
            std::map<std::string, int>& score_table);

        // Executes all games and fills score table
        void runAllGames(const std::vector<GameInput>& maps,
                        const std::vector<std::string>& algorithm_paths,
                        std::map<std::string, int>& score_table);
        
        bool validateRegistrars(const AlgorithmRegistrar& algo_registrar, const GameManagerRegistrar& gm_registrar) const; // Validates that at least two algorithms and one game manager are registered

        std::vector<CompetitionRunner::GameTask> createGameTasks(size_t num_maps, size_t algo_count); // Creates game tasks for all algorithm pairs on each map

        void runGamesMultiThreaded( const std::vector<GameTask>& tasks,  const std::vector<GameInput>& maps,
                                    const std::vector<std::string>& algorithm_paths, std::map<std::string, int>& score_table,
                                    int num_threads); // Runs games in parallel using multiple threads

        SatelliteViewImp createSatelliteView(const GameInput& map) const; // Creates a SatelliteViewImp from the game map

        void updateScore(std::map<std::string, int>& table, int winner, const std::string& a1, const std::string& a2) const; // Updates the score table based on the game result
            
        // Runs a single game between two algorithms on a given map
        void runSingleGame(const GameInput& map, int i, int j, 
            std::map<std::string, int>& score_table, 
            const std::vector<std::string>& algo_paths, std::mutex* score_mutex = nullptr);

        // Generates (i, j) algorithm index pairs for competition logic
        std::vector<std::pair<int, int>> generatePairs(int k, int N);

        // Writes the final scores to a timestamped result file
        void writeResults(const std::map<std::string, int>& score_table);

        // Sorts algorithms by total score descending
        std::vector<std::pair<std::string, int>> sortScores(const std::map<std::string, int>& scores);

        void printUsage(); // Prints usage instructions for the competition runner

    public:
        explicit CompetitionRunner(const ParsedArguments& args);
        void run();
};

#endif // COMPETITION_RUNNER_H