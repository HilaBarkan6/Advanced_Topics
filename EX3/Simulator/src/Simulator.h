#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "CommandLineParser.h"

class Simulator {
    private:
        ParsedArguments args;
            
    public:
        Simulator(const ParsedArguments& args);
        void run();

};

#endif