#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "CommandLineParser.h"
#include "ComparativeRunner.h"
#include "CompetitionRunner.h"
#include <iostream>
#include <memory>


class Simulator {
    private:
        ParsedArguments args;
        std::vector<void*> algo_handles;
        std::vector<void*> gm_handles;
            
    public:
        Simulator(const ParsedArguments& args);
        void run();
};

#endif