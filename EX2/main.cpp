#include "src/GameManager.h"
#include "src/DefensivePlayer.h"
#include "src/OffensivePlayer.h"
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
        GameManager m(std::move(player1), std::move(player2));
        m.readBoard(input_file);
        m.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Cannot recover from error, finishing game." << std::endl;
        return 1;
    }

    return 0;
}

