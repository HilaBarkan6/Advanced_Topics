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
            
    public:
        Simulator(const ParsedArguments& args);
        void run();

};

#endif