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

// Write a message directly to external.log with timestamp
void logToExternal(const std::string& message) {
    std::ofstream log_file("external.log", std::ios::app);
    if (!log_file.is_open()) return;

    std::time_t now = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

    log_file << buf << " [INFO] " << message << std::endl;
    log_file.flush();
}

// Finds the last Algorithm SO that started in the simulator log file
std::string findOffender(const std::string& logFile) {
    std::ifstream file(logFile);
    std::string line;
    std::string lastStart;
    while (std::getline(file, line)) {
        if (line.find("START_SO:") != std::string::npos &&
            line.find("Algorithm_") != std::string::npos) {

            auto pos = line.find("Algorithm_");
            if (pos != std::string::npos) {
                lastStart = line.substr(pos); // Algorithm_*_*.so
            }
        }
    }
    return lastStart;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ExternalManager <simulator_path> [simulator_args...]" << std::endl;
        return 1;
    }

    std::vector<std::string> original_args(argv + 1, argv + argc);

    // Detect algorithms_folder argument
    std::string algorithmsFolder;
    for (const auto& arg : original_args) {
        if (arg.rfind("algorithms_folder=", 0) == 0) { // starts with algorithms_folder=
            algorithmsFolder = arg.substr(strlen("algorithms_folder="));
            if (!algorithmsFolder.empty() && algorithmsFolder.back() == '/')
                algorithmsFolder.pop_back();
            logToExternal("Detected algorithms_folder: " + algorithmsFolder);
            break;
        }
    }

    if (algorithmsFolder.empty()) {
        logToExternal("ERROR: algorithms_folder argument not provided. Cannot proceed.");
        std::cerr << "ERROR: Missing required argument algorithms_folder=<path>" << std::endl;
        return 1;
    }

    while (true) {
        std::vector<char*> exec_args;
        for (auto& s : original_args) {
            exec_args.push_back(s.data());
        }
        exec_args.push_back(nullptr);

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            return 1;
        }

        if (pid == 0) {
            execvp(exec_args[0], exec_args.data());
            perror("execvp failed");
            exit(1);
        } else {
            int status;
            waitpid(pid, &status, 0);

            if (WIFSIGNALED(status)) {
                int sig = WTERMSIG(status);
                if (sig == SIGSEGV || sig == SIGABRT) {
                    std::cout << "Simulator crashed (signal " << sig << ")." << std::endl;
                    logToExternal("Simulator crashed (signal " + std::to_string(sig) + ")");

                    std::string offender = findOffender("simulator.log");
                    if (!offender.empty()) {
                        fs::create_directories("bad_algos");

                        fs::path offenderPath = fs::path(algorithmsFolder) / offender;
                        if (fs::exists(offenderPath)) {
                            fs::rename(offenderPath, fs::path("bad_algos") / offender);

                            std::cout << "Moved offending SO to bad_algos: " << offender << std::endl;
                            logToExternal("OFFENDING_SO_MOVED: " + offender + " -> bad_algos, restarting simulator...");
                        } else {
                            std::cout << "Offending SO not found: " << offender << std::endl;
                            logToExternal("WARNING: Offending SO not found in " + algorithmsFolder + ": " + offender);
                        }

                        original_args.erase(
                            std::remove_if(original_args.begin(), original_args.end(),
                                [&offender](const std::string& s){ return s.find(offender) != std::string::npos; }),
                            original_args.end()
                        );
                    } else {
                        std::cout << "No offending SO found in log." << std::endl;
                        logToExternal("WARNING: No offending SO found in simulator.log.");
                        break;
                    }

                    std::cout << "Restarting simulator without the offending SO..." << std::endl;
                    logToExternal("Restarting simulator without the offending SO...");
                    continue;
                }
            }

            break; // child exited normally
        }
    }

    std::cout << "Simulator finished successfully." << std::endl;
    logToExternal("Simulator finished successfully.");
    return 0;
}
