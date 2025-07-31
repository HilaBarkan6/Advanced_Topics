#include "Simulator.h"
#include "CommandLineParser.h"
#include "../../UserCommon/configuration/Config.h"

int main(int argc, char* argv[]){
    
    try{

        // Load configuration
        if (!Config::getInstance().loadFromFile("/mnt/c/Projects/Advanced_Topics/EX3/UserCommon/configuration/config.txt")) {
            std::cerr << "Warning: Could not load config.txt, using default values." << std::endl;
        }

        // Parse command line arguments
        ParsedArguments args = CommandLineParser::parse(argc, argv);

        Simulator simulator(args);
        simulator.run();
        
        
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        std::cerr << "Cannot recover from error, finishing simulator." << std::endl;
        return 1;
    }

    return 0;
}