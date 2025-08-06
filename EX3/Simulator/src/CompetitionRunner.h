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

class CompetitionRunner {

    private:
        ParsedArguments args;

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
        
        SatelliteViewImp createSatelliteView(const GameInput& map) const; // Creates a SatelliteViewImp from the game map

        void updateScore(std::map<std::string, int>& table, int winner, const std::string& a1, const std::string& a2) const; // Updates the score table based on the game result
            
        // Runs a single game between two algorithms on a given map
        void runSingleGameAndScore(const GameInput& map, int i, int j, 
            std::map<std::string, int>& score_table, 
            const std::vector<std::string>& algo_paths);

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