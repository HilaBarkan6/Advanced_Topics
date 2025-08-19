#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <ctime>
#include <cstring>

namespace fs = std::filesystem;

// -------------------- Logging --------------------
void logToExternal(const std::string& message) {
    std::ofstream log_file("external.log", std::ios::app);
    if (!log_file.is_open()) return;

    std::time_t now = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

    log_file << buf << " [INFO] " << message << std::endl;
    log_file.flush();
}

// -------------------- Offender Detection --------------------
std::string findOffender(const std::string& logFile) {
    std::ifstream file(logFile);
    std::string line, lastStart;
    while (std::getline(file, line)) {
        if (line.find("START_SO:") != std::string::npos &&
            line.find("Algorithm_") != std::string::npos) {

            auto pos = line.find("Algorithm_");
            if (pos != std::string::npos) lastStart = line.substr(pos);
        }
    }
    return lastStart;
}

// -------------------- Helper Functions --------------------
std::string parseAlgorithmsFolder(const std::vector<std::string>& args) {
    for (const auto& arg : args) {
        if (arg.rfind("algorithms_folder=", 0) == 0) {
            std::string folder = arg.substr(strlen("algorithms_folder="));
            if (!folder.empty() && folder.back() == '/') folder.pop_back();
            logToExternal("Detected algorithms_folder: " + folder);
            return folder;
        }
    }
    logToExternal("ERROR: algorithms_folder argument not provided. Cannot proceed.");
    std::cerr << "ERROR: Missing required argument algorithms_folder=<path>" << std::endl;
    return {};
}

int runSimulator(std::vector<std::string>& args) {
    std::vector<char*> exec_args;
    for (auto& s : args) exec_args.push_back(s.data());
    exec_args.push_back(nullptr);

    pid_t pid = fork();
    if (pid < 0) { perror("fork failed"); return -1; }
    if (pid == 0) { execvp(exec_args[0], exec_args.data()); perror("execvp failed"); exit(1); }

    int status; waitpid(pid, &status, 0);
    return status;
}

bool handleCrash(int status, std::vector<std::string>& args, const std::string& folder) {
    if (!WIFSIGNALED(status)) return false;
    int sig = WTERMSIG(status);
    if (sig != SIGSEGV && sig != SIGABRT) return false;

    std::cout << "Simulator crashed (signal " << sig << ")." << std::endl;
    logToExternal("Simulator crashed (signal " + std::to_string(sig) + ")");

    std::string offender = findOffender("simulator.log");
    if (offender.empty()) {
        std::cout << "No offending SO found in log." << std::endl;
        logToExternal("WARNING: No offending SO found in simulator.log.");
        return false;
    }

    fs::create_directories("bad_algos");
    fs::path offenderPath = fs::path(folder) / offender;
    if (fs::exists(offenderPath)) {
        fs::rename(offenderPath, fs::path("bad_algos") / offender);
        std::cout << "Moved offending SO to bad_algos: " << offender << std::endl;
        logToExternal("OFFENDING_SO_MOVED: " + offender + " -> bad_algos, restarting...");
    } else {
        std::cout << "Offending SO not found: " << offender << std::endl;
        logToExternal("WARNING: Offending SO not found in " + folder + ": " + offender);
    }

    args.erase(std::remove_if(args.begin(), args.end(),
                [&offender](const std::string& s){ return s.find(offender) != std::string::npos; }),
                args.end());
    std::cout << "Restarting simulator without the offending SO..." << std::endl;
    logToExternal("Restarting simulator without the offending SO...");
    return true;
}

// -------------------- Main --------------------
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ExternalManager <simulator_path> [simulator_args...]" << std::endl;
        return 1;
    }

    std::vector<std::string> args(argv + 1, argv + argc);
    std::string folder = parseAlgorithmsFolder(args);
    if (folder.empty()) return 1;

    while (true) {
        int status = runSimulator(args);
        if (status == -1 || !handleCrash(status, args, folder)) break;
    }

    std::cout << "Simulator finished successfully." << std::endl;
    logToExternal("Simulator finished successfully.");
    return 0;
}