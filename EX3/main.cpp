#include "src/GameManager.h"
#include "src/implementations/SimpleTankAlgorithmFactory.h"
#include "src/implementations/SimplePlayerFactory.h"
#include "src/configuration/Config.h"
#include <iostream>
#include <memory>
#include <string>


int main(int argc, char* argv[]){
    if(argc<2){
        std::cerr << "Please provide the input file path as a command line argument." << std::endl;
        return 1;
    }
    const std::string input_file = argv[1];
    try{

        // Load configuration
        if (!Config::getInstance().loadFromFile("src/configuration/config.txt")) {
            std::cerr << "Warning: Could not load config.txt, using default values." << std::endl;
        }
        
        // Initialize GameManager with factories
        GameManager m(std::make_unique<SimplePlayerFactory>(), std::make_unique<SimpleTankAlgorithmFactory>());
        m.readBoard(input_file);
        m.run();
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        std::cerr << "Cannot recover from error, finishing game." << std::endl;
        return 1;
    }

    return 0;
}
