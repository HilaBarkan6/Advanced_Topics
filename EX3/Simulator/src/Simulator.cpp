#include "Simulator.h"
#include "ComparativeRunner.h"
#include "CompetitionRunner.h"
#include <iostream>
#include <memory>

Simulator::Simulator(const ParsedArguments& args) : args(args) {}

void Simulator::run() {
    try {
        if (args.mode == RunMode::Comparative) {
            ComparativeRunner runner(args);
            runner.run();
        } else if (args.mode == RunMode::Competition) {
            CompetitionRunner runner(args);
            runner.run();
        } else {
            std::cerr << "Error: unknown run mode\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Simulator error: " << e.what() << std::endl;
    }
}