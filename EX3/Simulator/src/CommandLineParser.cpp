#include "CommandLineParser.h"

static std::string trim(const std::string& s) {
    auto start = s.begin(), end = s.end();
    while (start != end && std::isspace(*start)) ++start;
    while (start != end && std::isspace(*(end - 1))) --end;
    return std::string(start, end);
}

static void parseToken(const std::string& token, ParsedArguments& args, std::unordered_map<std::string, std::string>& kv) {
    if (token == "-comparative") args.mode = RunMode::Comparative;
    else if (token == "-competition") args.mode = RunMode::Competition;
    else if (token == "-verbose") args.verbose = true;
    else {
        auto eq = token.find('=');
        if (eq == std::string::npos)
            throw std::invalid_argument("Unknown argument: " + token);
        std::string key = trim(token.substr(0, eq));
        std::string val = trim(token.substr(eq + 1));
        kv[key] = val;
    }
}

static void fillArgs(ParsedArguments& args, const std::unordered_map<std::string, std::string>& kv) {
    if (args.mode == RunMode::Comparative) {
        if (!kv.count("game_map") || !kv.count("game_managers_folder") || !kv.count("algorithm1") || !kv.count("algorithm2"))
            throw std::invalid_argument("Missing args for comparative mode");
        args.game_map = kv.at("game_map");
        args.game_managers_folder = kv.at("game_managers_folder");
        args.algorithm1 = kv.at("algorithm1");
        args.algorithm2 = kv.at("algorithm2");
    } else if (args.mode == RunMode::Competition) {
        if (!kv.count("game_maps_folder") || !kv.count("game_manager") || !kv.count("algorithms_folder"))
            throw std::invalid_argument("Missing args for competition mode");
        args.game_maps_folder = kv.at("game_maps_folder");
        args.game_manager = kv.at("game_manager");
        args.algorithms_folder = kv.at("algorithms_folder");
    } else {
        throw std::invalid_argument("Missing run mode");
    }

    if (kv.count("num_threads")) {
        args.num_threads = std::stoi(kv.at("num_threads"));
        if (args.num_threads < 1)
            throw std::invalid_argument("num_threads must be >= 1");
    }
}

ParsedArguments CommandLineParser::parse(int argc, char* argv[]) {
    ParsedArguments args;
    std::unordered_map<std::string, std::string> kv;

    for (int i = 1; i < argc; ++i)
        parseToken(argv[i], args, kv);

    fillArgs(args, kv);
    return args;
}