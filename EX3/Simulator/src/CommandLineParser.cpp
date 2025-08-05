#include "CommandLineParser.h"

static void validateFolder(const std::string& path, const std::string& description) {
    std::filesystem::path folder(path);

    if (!std::filesystem::exists(folder))
        throw std::invalid_argument(description + " path does not exist: " + path);
    if (!std::filesystem::is_directory(folder))
        throw std::invalid_argument(description + " is not a directory: " + path);

    bool hasFile = false;
    for (const auto& entry : std::filesystem::directory_iterator(folder)) {
        if (std::filesystem::is_regular_file(entry)) {
            hasFile = true;
            break;
        }
    }

    if (!hasFile)
        throw std::invalid_argument(description + " contains no valid files: " + path);
}

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
    std::unordered_set<std::string> validKeys;
    std::unordered_set<std::string> requiredKeys;
    if (args.mode == RunMode::Comparative) {
        validKeys = {"game_map", "game_managers_folder", "algorithm1", "algorithm2", "num_threads"};
        requiredKeys = {"game_map", "game_managers_folder", "algorithm1", "algorithm2"};
    
        for (const auto& key: requiredKeys) {
            if (!kv.count(key))
                throw std::invalid_argument("Missing required argument for comparative mode: " + key);
        }
        validateFolder(args.game_managers_folder, "Game managers folder");
        args.game_map = kv.at("game_map");
        args.game_managers_folder = kv.at("game_managers_folder");
        args.algorithm1 = kv.at("algorithm1");
        args.algorithm2 = kv.at("algorithm2");

    } else if (args.mode == RunMode::Competition) {
        validKeys = {"game_maps_folder", "game_manager", "algorithms_folder", "num_threads"};
        requiredKeys = {"game_maps_folder", "game_manager", "algorithms_folder"};
        for (const auto& key: requiredKeys) {
            if (!kv.count(key))
                throw std::invalid_argument("Missing required argument for competition mode: " + key);
        }
        validateFolder(args.game_maps_folder, "Game maps folder");
        validateFolder(args.algorithms_folder, "Algorithms folder");
        args.game_maps_folder = kv.at("game_maps_folder");
        args.game_manager = kv.at("game_manager");
        args.algorithms_folder = kv.at("algorithms_folder");
    } else {
        throw std::invalid_argument("Missing run mode");
    }

     // Check for unsupported keys
    std::vector<std::string> unsupported;
    for (const auto& [key, _] : kv) {
        if (!validKeys.count(key)) {
            unsupported.push_back(key);
        }
    }
    if (!unsupported.empty()) {
        std::ostringstream oss;
        oss << "Unsupported arguments:";
        for (const auto& k : unsupported) oss << " " << k;
        throw std::invalid_argument(oss.str());
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