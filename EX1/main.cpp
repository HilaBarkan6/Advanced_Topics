#include "src/GameManager.h"
#include "src/SimplePlayer.h"
#include <iostream>


int main(int argc, char* argv[]){
    if(argc<2){
        std::cerr << "Please provide the input file path as a command line argument." << std::endl;
        return 1;
    }
    Player* player1 = new SimplePlayer(); 
    Player* player2 = new SimplePlayer();
    const std::string input_file = argv[1];
    const std::string output_file = "output/output.txt";
    GameManager* m = new GameManager(player1, player2, input_file, output_file);
    m->runGame();


    delete m;
    delete player1;
    delete player2;
    return 0;
}
