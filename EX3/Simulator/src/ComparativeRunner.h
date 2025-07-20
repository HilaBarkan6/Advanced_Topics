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

class ComparativeRunner {
public:
    explicit ComparativeRunner(const ParsedArguments& args);
    void run();

private:
    ParsedArguments args;

    GameInput tryReadMap(const std::string& path);

    // Loads two algorithm shared libraries (.so files). throws runtime_error if any of the libraries fail to load.
    std::vector<void*> tryLoadAlgorithms(const std::string& a1, const std::string& a2);

    // Scans the given folder for all .so files representing GameManagers. returns a vector of full paths to these files.
    std::vector<std::string> findGameManagers(const std::string& folder);

    std::map<std::string, std::set<std::string>> runAllGames(const std::vector<std::string>& gm_paths, const GameInput& input);

    // Writes the aggregated results to an output file in the GameManager folder.
    void writeResults(const std::map<std::string, std::set<std::string>>& result_map, const GameInput& input);

    // Converts a GameResult struct to a formatted string for output and comparison.
    std::string formatResult(const GameResult& r);

    // Joins a set of strings into a single comma-separated string.
    std::string join(const std::set<std::string>& items);
};

#endif