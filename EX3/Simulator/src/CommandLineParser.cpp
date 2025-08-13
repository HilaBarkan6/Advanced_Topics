#include "CommandLineParser.h"

bool CommandLineParser::isFlagToken(const std::string& s) {
    return s == "-comparative" || s == "-competition" || s == "-verbose";
}

void CommandLineParser::mergeEqAtStart(std::vector<std::string>& toks, const std::string& s) {
    if (toks.empty())
        throw std::invalid_argument("Invalid argument starting with '=' and no key: " + s);
    std::string &prev = toks.back();
    if (isFlagToken(prev) || prev.find('=') != std::string::npos)
        throw std::invalid_argument("Unexpected '=' token: " + s);
    prev += s; // merge to previous token
}

void CommandLineParser::mergeEqAtEnd(std::vector<std::string>& toks, const std::string& s, int& i, int argc, char* argv[]) {
    if (i + 1 >= argc)
        throw std::invalid_argument("Missing value after '=' for: " + s);
    toks.push_back(s + std::string(argv[i + 1]));
    i += 1;
}

void CommandLineParser::mergeSeparateEq(std::vector<std::string>& toks, const std::string& key, int& i, int argc, char* argv[]) {
    if (i + 2 >= argc)
        throw std::invalid_argument("Missing value after '=' for: " + key);
    toks.push_back(key + std::string("=") + std::string(argv[i + 2]));
    i += 2;
}

void CommandLineParser::mergeNextStartsWithEq(std::vector<std::string>& toks, const std::string& key, int& i, char* argv[]) {
    toks.push_back(key + std::string(argv[i + 1]));
    i += 1;
}

std::vector<std::string> CommandLineParser::canonicalizeArgs(int argc, char* argv[]) {
    std::vector<std::string> toks;
    for (int i = 1; i < argc; ++i) {
        std::string s = argv[i];

        if (isFlagToken(s)) { toks.push_back(s); continue; }

        auto eq_pos = s.find('=');
        if (eq_pos != std::string::npos) {
            if (eq_pos == 0) mergeEqAtStart(toks, s);
            else if (eq_pos == s.size() - 1) mergeEqAtEnd(toks, s, i, argc, argv);
            else toks.push_back(s); // "key=value"
            continue;
        }

        if (i + 1 < argc) {
            std::string next = argv[i + 1];
            if (next == "=") { mergeSeparateEq(toks, s, i, argc, argv); continue; }
            if (!next.empty() && next[0] == '=') { mergeNextStartsWithEq(toks, s, i, argv); continue; }
        }

        throw std::invalid_argument("Unknown or malformed Argument " + s);
    }
    return toks;
}

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

static void checkRequiredKeys(const std::unordered_map<std::string, std::string>& kv, const std::unordered_set<std::string>& requiredKeys, const std::string& modeName) {
    for (const auto& key : requiredKeys) {
        if (!kv.count(key))
            throw std::invalid_argument("Missing required argument for " + modeName + ": " + key);
    }
}

static void checkUnsupportedKeys(const std::unordered_map<std::string, std::string>& kv, const std::unordered_set<std::string>& validKeys) {
    std::vector<std::string> unsupported;
    for (const auto& [key, _] : kv) {
        if (!validKeys.count(key))
            unsupported.push_back(key);
    }

    if (!unsupported.empty()) {
        std::ostringstream oss;
        oss << "Unsupported arguments:";
        for (const auto& k : unsupported) oss << " " << k;
        throw std::invalid_argument(oss.str());
    }
}

static void fillArgs(ParsedArguments& args, const std::unordered_map<std::string, std::string>& kv) {
    std::unordered_set<std::string> validKeys, requiredKeys;

    if (args.mode == RunMode::Comparative) {
        validKeys = {"game_map", "game_managers_folder", "algorithm1", "algorithm2", "num_threads"};
        requiredKeys = {"game_map", "game_managers_folder", "algorithm1", "algorithm2"};
        checkRequiredKeys(kv, requiredKeys, "comparative mode");

        args.game_map = kv.at("game_map");
        args.game_managers_folder = kv.at("game_managers_folder");
        args.algorithm1 = kv.at("algorithm1");
        args.algorithm2 = kv.at("algorithm2");
        validateFolder(args.game_managers_folder, "Game managers folder");

    } else if (args.mode == RunMode::Competition) {
        validKeys = {"game_maps_folder", "game_manager", "algorithms_folder", "num_threads"};
        requiredKeys = {"game_maps_folder", "game_manager", "algorithms_folder"};
        checkRequiredKeys(kv, requiredKeys, "competition mode");

        args.game_maps_folder = kv.at("game_maps_folder");
        args.game_manager = kv.at("game_manager");
        args.algorithms_folder = kv.at("algorithms_folder");
        validateFolder(args.game_maps_folder, "Game maps folder");
        validateFolder(args.algorithms_folder, "Algorithms folder");

    } else {
        throw std::invalid_argument("Missing run mode");
    }

    checkUnsupportedKeys(kv, validKeys);

    if (kv.count("num_threads")) {
        args.num_threads = std::stoi(kv.at("num_threads"));
        if (args.num_threads < 1)
            throw std::invalid_argument("num_threads must be >= 1");
    }
}




ParsedArguments CommandLineParser::parse(int argc, char* argv[]) {
    ParsedArguments args;
    std::unordered_map<std::string, std::string> kv;

    // canonicalize raw argv into tokens like "-comparative" or "key=value"
    std::vector<std::string> tokens = canonicalizeArgs(argc, argv);

    for (const auto& t : tokens) {
        parseToken(t, args, kv);
    }

    fillArgs(args, kv);
    return args;
}