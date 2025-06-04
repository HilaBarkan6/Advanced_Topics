#include "src/GameManager.h"
#include "src/implementations/SimplePlayerFactory.h"
#include "src/implementations/SimpleTankAlgorithmFactory.h"
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
        Config& config = Config::getInstance();
        if (!config.loadFromFile("src/configuration\\config.txt")) {
            std::cerr << "Warning: Could not load config.txt, using default values." << std::endl;
        }

        // std::unique_ptr<PlayerFactory> player_factory(new SimplePlayerFactory());
        // std::unique_ptr<TankAlgorithmFactory> tank_algorithm_factory(new SimpleTankAlgorithmFactory());
        //GameManager m(std::move(player_factory), std::move(tank_algorithm_factory));
        GameManager m(config, std::make_unique<SimplePlayerFactory>(config), std::make_unique<SimpleTankAlgorithmFactory>(config));
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
