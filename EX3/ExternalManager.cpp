#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

namespace fs = std::filesystem;

// Finds the last Algorithm SO that started in the log file
std::string findOffender(const std::string& logFile) {
    std::ifstream file(logFile);
    std::string line;
    std::string lastStart;
    while (std::getline(file, line)) {
        if (line.find("START_SO: Algorithm_") != std::string::npos) {
            // extract only the filename
            auto pos = line.find("Algorithm_");
            lastStart = line.substr(pos); // Algorithm_*_*.so
        }
    }
    return lastStart;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ExternalManager <simulator_path> [simulator_args...]" << std::endl;
        return 1;
    }

    // Copy original arguments into a vector of strings for easy manipulation
    std::vector<std::string> original_args(argv + 1, argv + argc);

    while (true) {
        // Build argv array for execvp from original_args
        std::vector<char*> exec_args;
        for (auto& s : original_args) {
            exec_args.push_back(s.data());
        }
        exec_args.push_back(nullptr); // execvp requires nullptr-terminated array

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            return 1;
        }

        if (pid == 0) {
            // Child process: execute simulator
            execvp(exec_args[0], exec_args.data());
            perror("execvp failed"); // only reached if execvp fails
            exit(1);
        } else {
            // Parent process: wait for child
            int status;
            waitpid(pid, &status, 0);

            if (WIFSIGNALED(status)) {
                int sig = WTERMSIG(status);
                if (sig == SIGSEGV || sig == SIGABRT) {
                    std::cout << "Simulator crashed (signal " << sig << ")." << std::endl;

                    std::string offender = findOffender("simulator.log");
                    if (!offender.empty()) {
                        fs::create_directories("quarantine");

                        // Move offending SO to quarantine
                        fs::path offenderPath = fs::path("run/algos") / offender;
                        if (fs::exists(offenderPath)) {
                            fs::rename(offenderPath, fs::path("bad_algos") / offender);
                            std::cout << "Moved offending SO to quarantine: " << offender << std::endl;
                        } else {
                            std::cout << "Offending SO not found: " << offender << std::endl;
                        }

                        // Remove offending SO from original_args so it won't run next time
                        original_args.erase(
                            std::remove_if(original_args.begin(), original_args.end(),
                                [&offender](const std::string& s){ return s.find(offender) != std::string::npos; }),
                            original_args.end()
                        );
                    }

                    std::cout << "Restarting simulator without the offending SO..." << std::endl;
                    continue; // try again
                }
            }

            // Child exited normally or with a different signal
            break;
        }
    }

    std::cout << "Simulator finished successfully." << std::endl;
    return 0;
}
