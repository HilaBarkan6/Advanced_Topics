#ifndef COMMAND_LINE_PARSER_H
#define COMMAND_LINE_PARSER_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <cctype>

enum class RunMode { Comparative, Competition };

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
        static void printUsage();

    private:
        static bool isFlagToken(const std::string& s);
        static void mergeEqAtStart(std::vector<std::string>& toks, const std::string& s);
        static void mergeEqAtEnd(std::vector<std::string>& toks, const std::string& s, int& i, int argc, char* argv[]);
        static void mergeSeparateEq(std::vector<std::string>& toks, const std::string& key, int& i, int argc, char* argv[]);
        static void mergeNextStartsWithEq(std::vector<std::string>& toks, const std::string& key, int& i, char* argv[]);
        static std::vector<std::string> canonicalizeArgs(int argc, char* argv[]);

};

#endif // COMMAND_LINE_PARSER_H