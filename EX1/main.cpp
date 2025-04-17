#include "src/GameManager.h"
#include "src/SimplePlayer.h"
#include "src/SecondPlayer.h"
#include <iostream>


int main(int argc, char* argv[]){
    if(argc<2){
        std::cerr << "Please provide the input file path as a command line argument." << std::endl;
        return 1;
    }
    Player* player1 = new SimplePlayer(1); 
    Player* player2 = new SecondPlayer(2);
    const std::string input_file = argv[1];
    const std::string output_file = "output/output.txt";
    try{
        GameManager* m = new GameManager(player1, player2, input_file, output_file);
        m->runGame();
        delete m;
    }
    catch (const std::exception& e) {
        std::cerr << "Cannot recover from error, finishing game." << std::endl;
        return 1;
    }
    

    return 0;
}
