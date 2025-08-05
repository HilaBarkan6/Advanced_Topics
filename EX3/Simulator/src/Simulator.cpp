#include "Simulator.h"

Simulator::Simulator(const ParsedArguments& args) : args(args) {}

void Simulator::run() {
    try {
        if (args.mode == RunMode::Comparative) {
            ComparativeRunner runner(args);
            runner.run();
            algo_handles = runner.algo_handles; // Store handles for cleanup
            gm_handles = runner.gm_handles; // Store GameManager handles for cleanup
        } 
        else if (args.mode == RunMode::Competition) {
            CompetitionRunner runner(args);
            runner.run();
        } else {
            std::cerr << "Error: unknown run mode\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Simulator error: " << e.what() << std::endl;
    }
}
void Simulator::unloadSharedLibraries(const std::vector<void*>& handles) {
    for (void* handle : handles) {
        if (handle != nullptr) {
            dlclose(handle);
        }
    }
}

Simulator::~Simulator() {
    unloadSharedLibraries(algo_handles); // Clean up loaded algorithm handles
    unloadSharedLibraries(gm_handles); // Clean up loaded GameManager handles
}