#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#include "Board.h"
#include "Player.h"
#include "game_objects/Empty.h"
#include "game_objects/Wall.h"
#include "game_objects/Mine.h"
#include "game_objects/Tank.h"
#include "game_objects/Shell.h"
#include <vector>
#include <string>
#include <utility> 

class GameManager
{
    private:
        Player* player1;
        Player* player2;
        std::string path_input_file;
        std::string path_output_file;
        Board board;
        bool game_over;
        int width;
        int height;
        std::pair<bool,bool> checkCollisions(Tank tank1, Tank tank2, std::pair<int,int> tank1_new_location, std::pair<int,int> tank2_new_location, std::ofstream& output_file);
        bool isGameOver(std::ofstream& output_file);
        void updateExplodedObjects();
        void document();

    public:
        GameManager(Player* player1, Player* player2, const std::string& pathInputFile, const std::string& pathOutputFile);
        ~GameManager();
        void prepareGame();
        void runGame();
        void finishGame();
        void printBoard();
        std::pair<int, int> getNewLocation(const Tank& tank_to_move, Player::Action wanted_action);

};


#endif