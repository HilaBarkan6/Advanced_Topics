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
#include "configuration/Config.h"
#include "Logger/Logger.h"
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
        return std::hash<T1>()(pair.first) ^ (std::hash<T2>()(pair.second+10000) << 1);
    }
};

class GameManager {
    private:
        // Config parameters, default values, can be changed in config file
        static constexpr const char OUT_OF_BOUNDS_SIGN = '&';
        static constexpr const char WALL_SIGN = '#';
        static constexpr const char TANK1_SIGN = '1';
        static constexpr const char TANK2_SIGN = '2';
        static constexpr const char CALLED_TANK_SIGN = '%';
        static constexpr const char SHELL_SIGN = '*';
        static constexpr const char MINE_SIGN = '@';
        static constexpr const int MAX_TURNS_NO_SHELLS = 40;
        static constexpr const int WALL_LIVES = 2; 
        static constexpr const int BACKWARD_WAITING_TURNS = 2; 
        static constexpr const int SHOOTING_WAITING_TURNS = 4; 

        const Config& config;
        char out_of_bounds_sign;
        char wall_sign;
        char tank1_sign;
        char tank2_sign;
        char called_tank_sign;
        char shell_sign;
        char mine_sign;
        int max_turns_no_shells; // Used to check if game is over when no more shells are available.
        int wall_lives;
        int backward_wating_turns;
        int shooting_waiting_turns;

        Logger logger;

        std::string path_output_file;
        std::string path_log_file;
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
        std::vector<std::shared_ptr<Tank>> all_tanks;
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
        bool no_more_shells; // Used to check if both players have no shells left.
        
        // Satellite view is a single instance holding refrences to board, tanks and flying shells.
        // This single object will pass to players when needed.
        // NOTE: 'view' must be declared after 'board', 'all_tanks', and 'flying_shells' to avoid -Werror=reorder.
        SatelliteViewImp view;
        int counter_no_shells;

        bool shellFinished();

        bool isGameOver(std::ofstream& output_file);

        std::vector<std::vector<char>> createSatelliteMatrix() const;
        // Updates location for flying shells, is called every game iteration
        void MoveShells(bool is_even_turn);
        void updateShellNextLocation(std::shared_ptr<Shell> & shell);
        // Given the players wanted action, returns the tank's new location if it will be applied.
        std::pair<int, int> getNewLocation(const std::shared_ptr<Tank>& tank_to_move, ActionRequest wanted_action);

        std::set<std::shared_ptr<Tank>> tanks_to_kill;
        
        // Given the tanks new wanted locations, checks collisions between all relevant objects.
        std::unordered_map<int,bool> checkCollisions(std::unordered_map<int, std::pair<int, int>> new_wanted_locations);
        void applyAction(int tank_index, ActionRequest action, bool can_move, std::pair<int, int> new_location, const std::unordered_map<int, std::pair<int, int>>& new_wanted_locations ,std::ofstream& output_file);
        bool canMoveBackward(int tank_index) const;
        CanonDirection rotate(CanonDirection cur_dir, int rotation);
        std::pair<int, int> getShellLocationOnCreation(const std::shared_ptr<Tank>& tank_to_shoot) const;
        
        // Used to delete collided shells every iteration
        std::set<std::shared_ptr<Shell>> shells_to_delete;
        void deleteCollidedShells();
        void killTank(std::shared_ptr<Tank>& tank_to_kill);
        
        void readGameParameters(std::ifstream& file);
        int readIntValueFromLine(const std::string& line, const std::string& key);
        void initializeGame(std::ofstream& output_file);
        void handleEvenTurn(std::ofstream& output_file);
        void handleOddTurn();

        // Helper functions for MoveShells
        void moveAndHandleShellCollisions(std::unordered_map<std::pair<int, int>, std::vector<std::shared_ptr<Shell>>, pair_hash>& shell_locations_map);
        void handleShellTankHits(const std::unordered_map<std::pair<int, int>, std::vector<std::shared_ptr<Shell>>, pair_hash>& shell_locations_map);
        void handleShellToShellCollisions(const std::unordered_map<std::pair<int, int>, std::vector<std::shared_ptr<Shell>>, pair_hash>& shell_locations_map);

        // Helper functions for checkCollisions
        void addTankToLocationMap(std::unordered_map<std::pair<int, int>, std::vector<std::shared_ptr<Tank>>, pair_hash>& map,
            int tank_index, const std::pair<int, int>& cur_location, const std::pair<int, int>& new_location);
        void checkWallCollision(int tank_index, const std::pair<int,int>& new_location, std::unordered_map<int,bool>& can_tank_move);
        void checkMineCollision(int tank_index, const std::pair<int,int>& new_location, std::unordered_map<int,bool>& can_tank_move);
        void checkShellCollision(int tank_index, const std::pair<int,int>& cur_location, const std::pair<int,int>& new_location, std::unordered_map<int,bool>& can_tank_move);
        void resolveTankCollisions(const std::unordered_map<std::pair<int, int>, std::vector<std::shared_ptr<Tank>>, pair_hash>& map);
        void applyTankKillResults(std::unordered_map<int,bool>& can_tank_move);

        // Helper functions for applyAction
        bool handleBackwardWaiting(int tank_index, ActionRequest action);
        void handleBattleInfo(int tank_index);
        bool handleMoveForward(int tank_index, bool can_move, std::pair<int,int> new_location);
        bool handleMoveBackward(int tank_index, bool can_move, std::pair<int,int> new_location);
        void handleRotation(int tank_index, ActionRequest action);
        bool handleShooting(int tank_index, const std::unordered_map<int, std::pair<int, int>>& new_wanted_locations);

    public:
        explicit GameManager(const Config& config, std::unique_ptr<PlayerFactory> player_factory, std::unique_ptr<TankAlgorithmFactory> tank_algorithm_factory);
        virtual ~GameManager() = default;
        void readBoard(const std::string& pathInputFile);
        void run();
        int getHeight() const { return height; }
        int getWidth() const { return width; }
        void addTank(int row, int col, int player_id);
};

#endif