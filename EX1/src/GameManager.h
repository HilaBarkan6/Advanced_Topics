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
#include <unordered_map>
#include <set>
#include <memory>

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const {
        return std::hash<T1>()(pair.first) ^ (std::hash<T2>()(pair.second) << 1);
    }
};

class GameManager
{
    private:
        std::string path_input_file;
        std::string path_output_file;

        std::unique_ptr<Player> player1;
        std::unique_ptr<Player> player2;
 
        /* Used to check if player can move backward according to the rules
         * first - counter since requesting backward
         * second - if last action preformed is backward movement
         * third - if player is waiting for backward move.
         * Used ChatGpt to create and use the tuples. prompt was - "How to store 3 variable with different types in a data structure." */
        std::tuple<int, bool, bool> player1_backwards_info;
        std::tuple<int, bool, bool> player2_backwards_info;

        // Used to check if player can shoot according to the rules
        int player1_last_shooting;
        int player2_last_shooting;
        Tank tank1;
        Tank tank2;

        Board board;
        int width;
        int height;

        int turn_counter;
        // True when all player's shell were shooted.
        bool no_more_shells;
        // if no_more_shells is true, game will finish when this is 40
        int counter_no_shells;

        bool isGameOver(std::ofstream& output_file);
        // Updates location for flying shells, is called every game iteration
        void MoveShells(bool is_even_turn, std::ofstream& output_file);
        void updateShellNextLocation(Shell &shell);
        // Given the players wanted action, returns the tank's new location if it will be applied.
        std::pair<int, int> getNewLocation(const Tank& tank_to_move, Player::Action wanted_action);
        // Given the tanks new wanted locations, checks collisions between all relevant objects.
        std::pair<bool,bool> checkCollisions(std::pair<int,int> tank1_new_location, std::pair<int,int> tank2_new_location, std::ofstream& output_file);
        void applyAction(Tank& tank_to_apply, Player::Action action, bool can_move, std::pair<int, int> new_location, std::ofstream& output_file);
        bool canMoveBackward(int player_id) const;
        CanonDirection rotate(CanonDirection cur_dir, int rotation);
        std::pair<int, int> getShellLocationOnCreation(const Tank& tank_to_shoot) const;
        
        // Used to delete collided shells every iteration
        std::set<Shell *> shells_to_delete;
        void deleteCollidedShells();

    public:
        GameManager(std::unique_ptr<Player> player1, std::unique_ptr<Player> player2, const std::string& pathInputFile, const std::string& pathOutputFile);
        //~GameManager() = default;
        void runGame();
};

#endif