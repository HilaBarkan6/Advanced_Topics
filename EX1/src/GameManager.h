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
        Tank* tank1;
        Tank* tank2;
        std::string path_input_file;
        std::string path_output_file;
        Board board;
        bool game_over;
        int width;
        int height;
        std::pair<bool,bool> checkCollisions(std::pair<int,int> tank1_new_location, std::pair<int,int> tank2_new_location, std::ofstream& output_file);
        bool isGameOver(std::ofstream& output_file);
        void updateExplodedObjects();
        void document();
        std::tuple<int, bool, bool> player1_backwards_info;
        std::tuple<int, bool, bool> player2_backwards_info;
        int player1_last_shooting;
        int player2_last_shooting;
        int turn_counter;
        std::pair<int, int> getShellLocation(const Tank* const tank_to_shoot) const;
        bool no_more_shells;
        //game will finish when this is 40
        int counter_no_shells;

    public:
        GameManager(Player* player1, Player* player2, const std::string& pathInputFile, const std::string& pathOutputFile);
        ~GameManager();
        void prepareGame();
        void runGame();
        void finishGame();
        void printBoard();
        std::pair<int, int> getNewLocation(Player* player, Tank tank_to_move, Player::Action wanted_action);
        void applyAction(Player* player, Tank* tank, Player::Action action, bool can_move, std::pair<int, int> new_location, std::ofstream& output_file);
        bool canMoveBackward(Player* player) const;
        CanonDirection rotate(CanonDirection cur_dir, int rotation);
        void updateShellNextLocation(Shell shell);
};


#endif