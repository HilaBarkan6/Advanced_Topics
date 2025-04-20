#include "src/GameManager.h"
#include "src/SimplePlayer.h"
#include "src/SecondPlayer.h"
#include "src/DefensivePlayer.h"
#include "src/OffensivePlayer.h"
#include <iostream>


int main(int argc, char* argv[]){
    if(argc<2){
        std::cerr << "Please provide the input file path as a command line argument." << std::endl;
        return 1;
    }
    // Create players inherits from "Player" class, currently first defensive and second offensive.
    Player* player1 = new DefensivePlayer(1); 
    Player* player2  = new OffensivePlayer(2);
    const std::string input_file = argv[1];
    const std::string output_file = "output/output.txt";
    try{
        GameManager m(player1, player2, input_file, output_file);
        m.runGame();
    }
    catch (const std::exception& e) {
        std::cerr << "Cannot recover from error, finishing game." << std::endl;
        return 1;
    }
    

    return 0;
}
