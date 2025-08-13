#include <filesystem>
#include <iostream>
#include "Simulator.h"
#include "CommandLineParser.h"
#include "../../UserCommon/configuration/Config.h"

using namespace UserCommon_209399021_208239152;
namespace fs = std::filesystem;

// Tries to locate the configuration file in multiple reasonable locations
fs::path findConfigFile(const char* argv0) {
    try {
        // Attempt 1: Relative to the executable path
        fs::path exe_path = fs::canonical(fs::absolute(argv0));
        fs::path exe_dir = exe_path.parent_path();
        fs::path relative_config = exe_dir / "../../UserCommon/configuration/config.txt";
        if (fs::exists(relative_config)) {
            return fs::canonical(relative_config);
        }
    } catch (...) {
        // Ignore and try next option
    }

    // Attempt 2: Relative to the current working directory
    fs::path cwd_config = fs::current_path() / "UserCommon/configuration/config.txt";
    if (fs::exists(cwd_config)) {
        return fs::canonical(cwd_config);
    }

    // Config file not found
    throw std::runtime_error("Could not locate config.txt in expected paths.");
}

int main(int argc, char* argv[]) {
    try {
        // Try loading configuration from file
        try {
            fs::path config_path = findConfigFile(argv[0]);
            if (!Config::getInstance().loadFromFile(config_path)) {
                std::cerr << "Warning: Could not load config.txt from " << config_path << ", using default values." << std::endl;
            } 
        } 
        catch (const std::exception& e) {
            std::cerr << "Warning: " << e.what() << " – using default config values." << std::endl;
        }

        // Parse command-line arguments
        ParsedArguments args;
        try{
            args = CommandLineParser::parse(argc, argv);
        }
        catch (const std::exception& e) {
            std::cerr << "Error parsing command line arguments: " << e.what() << std::endl;
            CommandLineParser::printUsage();
            return 1;
        }

        // Run the simulator
        Simulator simulator(args);
        simulator.run();
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        std::cerr << "Cannot recover from error, finishing simulator." << std::endl;
        return 1;
    }

    return 0;
}