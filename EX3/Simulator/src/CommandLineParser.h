#ifndef COMMAND_LINE_PARSER_H
#define COMMAND_LINE_PARSER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <cctype>

enum class RunMode { Comparative, Competition };

//CR: Consider removing the struct and just use the class either with public members or with getters.
struct ParsedArguments {
    RunMode mode;
    std::string game_map;
    std::string game_maps_folder;
    std::string game_managers_folder;
    std::string game_manager;
    std::string algorithm1;
    std::string algorithm2;
    std::string algorithms_folder;
    int num_threads = 1;
    bool verbose = false;
};

class CommandLineParser {
public:
    static ParsedArguments parse(int argc, char* argv[]);
};

#endif // COMMAND_LINE_PARSER_H