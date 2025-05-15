#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#include "Board.h"
#include "common/Player.h"
#include "common/ActionRequest.h"
#include "game_objects/Empty.h"
#include "game_objects/Wall.h"
#include "game_objects/Mine.h"
#include "game_objects/Tank.h"
#include "game_objects/Shell.h"
#include "common/PlayerFactory.h"
#include "common/TankAlgorithmFactory.h"
#include "common/SatelliteView.h"
#include "implementations/SatelliteViewImp.h"
#include <vector>
#include <string>
#include <utility> 
#include <unordered_map>
#include <set>
#include <memory>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const {
        return std::hash<T1>()(pair.first) ^ (std::hash<T2>()(pair.second) << 1);
    }
};

class GameManager
{
    private:
        std::string path_output_file;
        std::unique_ptr<PlayerFactory> player_factory;
        std::unique_ptr<TankAlgorithmFactory> tank_algorithm_factory;
        std::unique_ptr<Player> player1;
        std::unique_ptr<Player> player2;
 
        /* Used to check if tank can move backward according to the rules. maps tank index to its relevant tuple.
         * first - counter since requesting backward
         * second - if last action preformed is backward movement
         * third - if player is waiting for backward move.
         * Used ChatGpt to create and use the tuples. prompt was - "How to store 3 variable with different types in a data structure." */
        std::unordered_map<int, std::tuple<int, bool, bool>> all_tanks_backwards_info;
        

        // Used to check if player can shoot according to the rules. maps tank index to their last shooting turn.
        std::unordered_map<int, int> tank_last_shooting;

        // Both player tanks in the order they were "born"
        std::vector<Tank> all_tanks;
        int player1_alive_tanks;
        int player2_alive_tanks;
        

        // Flying shells of all tanks
        std::vector<std::shared_ptr<Shell>> flying_shells;

        Board board;

        int max_steps;
        int num_shells;
        int width;
        int height;


        int turn_counter;
        // True when all player's shell were shooted.
        bool no_more_shells;
        // if no_more_shells is true, game will finish when this is 40
        int counter_no_shells;

        // Satellite view is a single instance holding refrences to board, tanks and flying shells.
        // This single object will pass to players when needed.
        SatelliteViewImp view;

        bool shellFinished();

        bool isGameOver(std::ofstream& output_file);

        std::vector<std::vector<char>> createSatelliteMatrix() const;
        // Updates location for flying shells, is called every game iteration
        void MoveShells(bool is_even_turn, std::ofstream& output_file);
        void updateShellNextLocation(std::shared_ptr<Shell> & shell);
        // Given the players wanted action, returns the tank's new location if it will be applied.
        std::pair<int, int> getNewLocation(const Tank& tank_to_move, ActionRequest wanted_action);

        std::set<Tank *> tanks_to_kill;
        
        // Given the tanks new wanted locations, checks collisions between all relevant objects.
        std::unordered_map<int,bool> checkCollisions(std::unordered_map<int, std::pair<int, int>> new_wanted_locations, std::ofstream& output_file);
        // TODO - adress new action of get battle info
        void applyAction(int tank_index, ActionRequest action, bool can_move, std::pair<int, int> new_location, const std::unordered_map<int, std::pair<int, int>>& new_wanted_locations ,std::ofstream& output_file);
        bool canMoveBackward(int tank_index) const;
        CanonDirection rotate(CanonDirection cur_dir, int rotation);
        std::pair<int, int> getShellLocationOnCreation(const Tank& tank_to_shoot) const;
        
        // Used to delete collided shells every iteration
        std::set<std::shared_ptr<Shell>> shells_to_delete;
        void deleteCollidedShells();

    public:
        GameManager(std::unique_ptr<PlayerFactory> player_factory, std::unique_ptr<TankAlgorithmFactory> tank_algorithm_factory);
        ~GameManager() = default;
        void readBoard(const std::string& pathInputFile);
        void run();
};

#endif