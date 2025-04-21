#include "src/GameManager.h"
#include "src/SimplePlayer.h"
#include "src/SecondPlayer.h"
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
    // Create players inherits from "Player" class, currently first defensive and second offensive.
    std::unique_ptr<Player> player1(new DefensivePlayer()); 
    std::unique_ptr<Player> player2(new OffensivePlayer());
    const std::string input_file = argv[1];
    // Used ChatGpt to extract file name and concat to output file name. prompt used "how to extract file name from a string that is file path"
    size_t last_slash = input_file.find_last_of("/\\");
    const std::string input_file_name = (last_slash == std::string::npos) ? input_file : input_file.substr(last_slash + 1);
    const std::string output_file = "output/output_"+input_file_name;
    try{
        GameManager m(std::move(player1), std::move(player2), input_file, output_file);
        m.runGame();
    }
    catch (const std::exception& e) {
        std::cerr << "Cannot recover from error, finishing game." << std::endl;
        return 1;
    }

    return 0;
}

