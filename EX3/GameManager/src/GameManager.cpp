#include "GameManager.h"

using namespace GameManager_209399021_208239152;
REGISTER_GAME_MANAGER(GameManager);

// Overload operator<< for ActionRequest
std::ostream& operator<<(std::ostream& os, const ActionRequest& action) {
    switch (action) {
        case ActionRequest::MoveForward: os << "MoveForward"; break;
        case ActionRequest::MoveBackward: os << "MoveBackward"; break;
        case ActionRequest::RotateLeft90: os << "RotateLeft90"; break;
        case ActionRequest::RotateRight45: os << "RotateRight45"; break;
        case ActionRequest::RotateRight90: os << "RotateRight90"; break;
        case ActionRequest::RotateLeft45: os << "RotateLeft45"; break;
        case ActionRequest::Shoot: os << "Shoot"; break;
        case ActionRequest::DoNothing: os << "DoNothing"; break;
        case ActionRequest::GetBattleInfo: os << "GetBattleInfo"; break;
        default: os << "UnknownAction"; break;
    }
    return os;
}


// Overload operator<< for CanonDirection
std::ostream& operator<<(std::ostream& os, const CanonDirection& direction) {
    switch (direction) {
        case CanonDirection::UP: os << "UP"; break;
        case CanonDirection::DOWN: os << "DOWN"; break;
        case CanonDirection::LEFT: os << "LEFT"; break;
        case CanonDirection::RIGHT: os << "RIGHT"; break;
        case CanonDirection::UP_RIGHT: os << "UP_RIGHT"; break;
        case CanonDirection::UP_LEFT: os << "UP_LEFT"; break;
        case CanonDirection::DOWN_LEFT: os << "DOWN_LEFT"; break;
        case CanonDirection::DOWN_RIGHT: os << "DOWN_RIGHT"; break;
        default: os << "UNKNOWN"; break;
    }
    return os;
}

GameManager::GameManager(bool verbose): 
      out_of_bounds_sign(Config::getInstance().get("out_of_bounds_sign", std::string(1, OUT_OF_BOUNDS_SIGN))[0]),
      wall_sign(Config::getInstance().get("wall_sign", std::string(1, WALL_SIGN))[0]),
      tank1_sign(Config::getInstance().get("tank1_sign", std::string(1, TANK1_SIGN))[0]),
      tank2_sign(Config::getInstance().get("tank2_sign", std::string(1, TANK2_SIGN))[0]),
      called_tank_sign(Config::getInstance().get("called_tank_sign", std::string(1, CALLED_TANK_SIGN))[0]),
      shell_sign(Config::getInstance().get("shell_sign", std::string(1, SHELL_SIGN))[0]),
      mine_sign(Config::getInstance().get("mine_sign", std::string(1, MINE_SIGN))[0]),
      max_turns_no_shells(Config::getInstance().getInt("max_turns_no_shells", MAX_TURNS_NO_SHELLS)),
      wall_lives(Config::getInstance().getInt("wall_lives", WALL_LIVES)),
      backward_wating_turns(Config::getInstance().getInt("backward_waiting_turns", BACKWARD_WAITING_TURNS)),
      shooting_waiting_turns(Config::getInstance().getInt("shooting_waiting_turns", SHOOTING_WAITING_TURNS)),
      should_verbose(verbose),
      player1_alive_tanks(0),
      player2_alive_tanks(0),
      turn_counter(0),
      no_more_shells(false),
      view(),
      counter_no_shells(0) {}




void GameManager::initializeGame(std::ofstream& output_file) {
    //this->view.setRowsAndColumns(height, width);
    //TODO - we receive the players in run function so we save them there
    // this->player1 = player_factory->create(1, width, height, max_steps, num_shells);
    // this->player2 = player_factory->create(2, width, height, max_steps, num_shells);

    for(size_t i = 0; i < all_tanks.size(); i++){
        tank_last_shooting[i] = -1;
        all_tanks_backwards_info[i] = std::make_tuple(0, false, false);
    }

    logger.setLogFile(path_log_file);

    if(should_verbose){
        output_file.open(path_output_file, std::ios::out);
        if (!output_file.is_open()) {
            logger.logError("Error opening output file: " + path_output_file);
            std::cerr << "Error opening output file: " << path_output_file << std::endl;
        }
    }

    view = std::make_unique<SatelliteViewImp>();
}

void GameManager::handleEvenTurn(std::ofstream& output_file) {
    MoveShells(true);

    std::unordered_map<int, ActionRequest> wanted_actions;
    std::unordered_map<int, std::pair<int, int>> new_wanted_locations;

    for (size_t i = 0; i < all_tanks.size(); i++) {
        if (all_tanks[i]->getAlive()) {
            wanted_actions[i] = all_tanks[i]->getTankAlgorithm().getAction();
            if(std::get<0>(all_tanks_backwards_info[i]) == backward_wating_turns &&
            std::get<2>(all_tanks_backwards_info[i]) &&
            wanted_actions[i] != ActionRequest::MoveForward &&
            wanted_actions[i] != ActionRequest::GetBattleInfo) {
                /* If the tank is waiting for backward move and it is the 3rd turn, and the action was not forward or get battle info,
                 its time to move so we ignore what the tank asked because the actual action is backward.
                 for more info about this behavior see readme */
                wanted_actions[i] = ActionRequest::MoveBackward;
            }
            new_wanted_locations[i] = getNewLocation(all_tanks[i], i, wanted_actions[i]);
        }
    }

    std::unordered_map<int, bool> can_move = checkCollisions(new_wanted_locations);
    deleteCollidedShells();

    for (size_t i = 0; i < all_tanks.size(); i++) {
        if (can_move.find(i) == can_move.end()) {
            if(should_verbose){
                output_file << "killed";
            }
        } else {
            applyAction(i, wanted_actions[i], can_move[i], new_wanted_locations[i], new_wanted_locations, output_file);
        }
        if (i != all_tanks.size() - 1) {
            if(should_verbose){
                output_file << ", ";
            }
            
        }
    }
    if(should_verbose){
        output_file << std::endl;
    }

    deleteCollidedShells();
}

void GameManager::handleOddTurn() {
    MoveShells(false);
    deleteCollidedShells();
}

void GameManager::createBoardAndTanksFromMap(const SatelliteView& map, size_t height, size_t width, TankAlgorithmFactory player1_tank_algo_factory, TankAlgorithmFactory player2_tank_algo_factory) {
    this->height = height;
    this->width = width;
    board = Board(height, width, wall_lives, wall_sign, tank1_sign, tank2_sign, mine_sign);
    view->setRowsAndColumns(height, width);

    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            char object = map.getObjectAt(i, j);
            if (object == wall_sign) {
                board.setGameObjectAt(i, j, std::make_unique<Wall>(wall_lives));
            } else if (object == mine_sign) {
                board.setGameObjectAt(i, j, std::make_unique<Mine>());
            } else if (object == tank1_sign){
                addTank(i, j, 1);
                //get the last tank in all_tanks vector and set its algorithm
                all_tanks.back()->setTankAlgorithm(player1_tank_algo_factory(1, player1_alive_tanks));    
            } else if (object == tank2_sign) {
                addTank(i, j, 2);
                //get the last tank in all_tanks vector and set its algorithm
                all_tanks.back()->setTankAlgorithm(player2_tank_algo_factory(2, player2_alive_tanks));
            }
        }
    }

}
    

GameResult GameManager::run(size_t map_width, size_t map_height,
                        const SatelliteView& map,
                        string map_name,
                        size_t max_steps, size_t num_shells,
                        Player& player1, string name1, Player& player2, string name2,
                        TankAlgorithmFactory player1_tank_algo_factory,
                        TankAlgorithmFactory player2_tank_algo_factory) 
    {
    //TODO - fix this, not sure how to take the players from the parameters and keep them locally in game manager as unique_ptrs
    this->player1 = &player1;
    this->player2 = &player2;
    this->max_steps = max_steps;
    this->num_shells = num_shells;

    std::string filename = map_name + "_" + name1 + "_" + name2 + ".txt";
    std::filesystem::create_directories("results");
    std::filesystem::create_directories("log_output");
    path_output_file = "results/output_" + filename;
    path_log_file = "log_output/log_" + filename;

    std::ofstream output_file;
    initializeGame(output_file);

    
    view->setRowsAndColumns(map_height, map_width);
    createBoardAndTanksFromMap(map, map_height, map_width, player1_tank_algo_factory, player2_tank_algo_factory);
    
    while (!isGameOver(output_file)) {
        view->setSatelliteView(createSatelliteMatrix());
        

        if (turn_counter % 2 == 0) {
            logger.logInfo("Turn is " + std::to_string(turn_counter / 2));
            handleEvenTurn(output_file);
        } else {
            handleOddTurn();
        }

        if (shellFinished()) {
            no_more_shells = true;
            counter_no_shells++;    
            logger.logInfo("Both players have no more shells, counting " + std::to_string(max_turns_no_shells) + " turns to finish");
        }
        turn_counter++;
    }
    return std::move(game_result);
}

std::vector<std::vector<char>> GameManager::createSatelliteMatrix() const {
    std::vector<std::vector<char>> satellite_matrix(height, std::vector<char>(width, ' '));

    // Add Walls and Mines
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (board.isMineLocation(i, j)) {
                satellite_matrix[i][j] = mine_sign;
            } else if (board.isWallLocation(i, j)) {
                satellite_matrix[i][j] = wall_sign;
            }
        }
    }

    // Add Tanks
    for (const auto& tank : all_tanks) {
        if (tank->getAlive()) {
            int x = tank->getLocationX();
            int y = tank->getLocationY();
            satellite_matrix[x][y] = (tank->getPlayerId() == 1) ? tank1_sign : tank2_sign;
        }
    }

    // Add Shells
    for (const auto& shell : flying_shells) {
        int x = shell->getLocation().first;
        int y = shell->getLocation().second;
        satellite_matrix[x][y] = shell_sign;
    }

    return satellite_matrix;
}


// Check if both players finished their shells for all tanks
bool GameManager::shellFinished(){
    for(size_t i=0; i<all_tanks.size(); i++){
        if(all_tanks[i]->getUnusedShellsCount() > 0){
            return false;
        }
    }
    return true;
}

void GameManager::FillGameResult(){
    game_result.rounds = turn_counter / 2;
    game_result.remaining_tanks.push_back(player1_alive_tanks);
    game_result.remaining_tanks.push_back(player2_alive_tanks);
    game_result.gameState = std::move(view);
}

bool GameManager::isGameOver(std::ofstream& output_file) {
    // Check if both players finished their shells and 40 turns passed
    if (player1_alive_tanks > 0  && player2_alive_tanks > 0) {
        if(no_more_shells && counter_no_shells>=2*max_turns_no_shells){
            
            if(should_verbose){
                output_file << "Tie, both players have zero shells for " << max_turns_no_shells << " steps";
            }
            logger.logInfo("Tie, both players have zero shells for " + std::to_string(max_turns_no_shells) + " steps");
            
            FillGameResult();
            game_result.winner = 0;
            game_result.reason = GameResult::ZERO_SHELLS;
            
            return true;
        }
        if(turn_counter >= max_steps*2){

            if(should_verbose){
                output_file << "Tie, reached max steps = " << max_steps << " , player 1 has " << player1_alive_tanks << " tanks, player 2 has " << player2_alive_tanks << " tanks";
            }

            logger.logInfo("Tie, reached max steps = " + std::to_string(max_steps) + " , player 1 has " + std::to_string(player1_alive_tanks) + " tanks, player 2 has " + std::to_string(player2_alive_tanks) + " tanks");
            FillGameResult();
            game_result.winner = 0;
            game_result.reason = GameResult::MAX_STEPS;
            return true;
        }
        return false; 
    }
    // Both players are dead
    if (player1_alive_tanks == 0  && player2_alive_tanks == 0) {
        if(should_verbose){
            output_file << "Tie, both players have zero tanks";
        }
        logger.logInfo("Tie, both players have zero tanks");

        FillGameResult();
        game_result.winner = 0;
        game_result.reason = GameResult::ALL_TANKS_DEAD;
        return true;
    }
    // Only player1 dead
    else if (player1_alive_tanks == 0) {
        if(should_verbose){
            output_file << "Player 2 won with " << player2_alive_tanks << " tanks still alive";
    }
        logger.logInfo("Player 2 won with " + std::to_string(player2_alive_tanks) + " tanks still alive");
        FillGameResult();
        game_result.winner = 2;
        game_result.reason = GameResult::ALL_TANKS_DEAD;

        return true;
    // Only player 2 dead
    } else if (player2_alive_tanks == 0) {
        if(should_verbose){
            output_file << "Player 1 won with " << player1_alive_tanks << " tanks still alive";
        }
        logger.logInfo("Player 1 won with " + std::to_string(player1_alive_tanks) + " tanks still alive");
        FillGameResult();
        game_result.winner = 1; 
        game_result.reason = GameResult::ALL_TANKS_DEAD;
        return true;
    }
    
    return true;  
}

void GameManager::moveAndHandleShellCollisions(std::unordered_map<std::pair<int, int>, std::vector<std::shared_ptr<Shell>>, pair_hash>& shell_locations_map) {
    for (auto& shell : flying_shells) {
        auto shell_cur_location = shell->getLocation();

        if (shell_locations_map.find(shell_cur_location) != shell_locations_map.end()) {
            shells_to_delete.insert(shell);
            shells_to_delete.insert(shell_locations_map[shell_cur_location].begin(), shell_locations_map[shell_cur_location].end());
            logger.logInfo("shells are colliding at location [" + std::to_string(shell_cur_location.first) + ", " + std::to_string(shell_cur_location.second) + "]");
        }

        int x = shell->getNextLocation().first;
        int y = shell->getNextLocation().second;

        if (board.isWallLocation(x, y)) {
            board.reduceWallLives(x, y);
            if (board.wallIsDestroyed(x, y)) {
                board.setGameObjectAt(x, y, std::make_unique<Empty>());
                logger.logInfo("Wall at [" + std::to_string(x) + ", " + std::to_string(y) + "] destroyed");
            }
            shells_to_delete.insert(shell);
            continue;
        }

        shell->setPrevLocation(shell->getLocation());
        shell->setLocation(shell->getNextLocation());
        updateShellNextLocation(shell);
        shell_locations_map[shell->getLocation()].push_back(shell);
    }
}

void GameManager::handleShellTankHits(const std::unordered_map<std::pair<int, int>, std::vector<std::shared_ptr<Shell>>, pair_hash>& shell_locations_map) {
    for (const auto& [loc, _] : shell_locations_map) {
        for (size_t i = 0; i < all_tanks.size(); i++) {
            if (loc.first == all_tanks[i]->getLocationX() && loc.second == all_tanks[i]->getLocationY() && all_tanks[i]->getAlive()) {
                if (all_tanks[i]->getPlayerId() == 1) {
                    player1_alive_tanks--;
                    logger.logInfo("Player 1 tank" + std::to_string(i) + " at [" + std::to_string(loc.first) + ", " + std::to_string(loc.second) + "] hit by shell");
                } else {
                    player2_alive_tanks--;
                    logger.logInfo("Player 2 tank" + std::to_string(i) + " at [" + std::to_string(loc.first) + ", " + std::to_string(loc.second) + "] hit by shell");
                }
                all_tanks[i]->setAlive();
            }
        }
    }
}

void GameManager::handleShellToShellCollisions(const std::unordered_map<std::pair<int, int>, std::vector<std::shared_ptr<Shell>>, pair_hash>& shell_locations_map) {
    for (const auto& [loc, vec] : shell_locations_map) {
        if (vec.size() > 1) {
            logger.logInfo(std::to_string(vec.size()) + " shells collided at location [" + std::to_string(loc.first) + ", " + std::to_string(loc.second) + "]");
            for (const auto& shell : vec) {
                shells_to_delete.insert(shell);
            }
        }
    }
}

void GameManager::MoveShells(bool is_even_turn) {
    std::unordered_map<std::pair<int, int>, std::vector<std::shared_ptr<Shell>>, pair_hash> shell_locations_map;

    moveAndHandleShellCollisions(shell_locations_map);
    
    if (!is_even_turn) {
        handleShellTankHits(shell_locations_map);
    }

    handleShellToShellCollisions(shell_locations_map);
}

void GameManager::updateShellNextLocation(std::shared_ptr<Shell> & shell){
    int dx = 0;
    int dy = 0;
    CanonDirection dir = shell->getFlyingDirection();
    // Calculate dx and dy according to the shell flying direction
    // Used ChatGpt to calculate deltas. prompt was "Given the CanonDirection of the shell calculate next location"
    switch(dir){
        case CanonDirection::UP: dx = -1; dy = 0; break;
        case CanonDirection::DOWN: dx = 1; dy = 0; break;
        case CanonDirection::LEFT: dx = 0; dy = -1; break;
        case CanonDirection::RIGHT: dx = 0; dy = 1; break;
        case CanonDirection::UP_RIGHT: dx = -1; dy = 1; break;
        case CanonDirection::UP_LEFT: dx = -1; dy = -1; break;
        case CanonDirection::DOWN_LEFT : dx = 1; dy = -1; break;
        case CanonDirection::DOWN_RIGHT: dx = 1; dy = 1; break;
        default: break;
    }
    // If shell arriving to the board border, continue from the other size, board is circular
    int x_location = (shell->getLocation().first + dx + height) % height;
    int y_location = (shell->getLocation().second + dy + width) % width;
    shell->setNextLocation(std::make_pair(x_location, y_location));
}

std::pair<int, int> GameManager::getNewLocation(const std::shared_ptr<Tank>& tank_to_move, int tank_index, ActionRequest wanted_action){
    int dx = 0;
    int dy = 0;
    CanonDirection dir = tank_to_move->getCanonDirection();
    // Forward and Backward are the only actions that move a tank, in other action location stays the same
    // Used ChatGpt to calculate deltas. prompt was "Given the CanonDirection of the tank and action that is forward or backward, calculate next location"
    if(wanted_action == ActionRequest::MoveForward){
        switch(dir){
            case CanonDirection::UP: dx = -1; dy = 0; break;
            case CanonDirection::DOWN: dx = 1; dy = 0; break;
            case CanonDirection::LEFT: dx = 0; dy = -1; break;
            case CanonDirection::RIGHT: dx = 0; dy = 1; break;
            case CanonDirection::UP_RIGHT: dx = -1; dy = 1; break;
            case CanonDirection::UP_LEFT: dx = -1; dy = -1; break;
            case CanonDirection::DOWN_LEFT : dx = 1; dy = -1; break;
            case CanonDirection::DOWN_RIGHT: dx = 1; dy = 1; break;
            default: break;
        }
    }
    else if (wanted_action == ActionRequest::MoveBackward && canMoveBackward(tank_index)){
        switch(dir){
            case CanonDirection::UP: dx = 1; dy = 0; break;
            case CanonDirection::DOWN: dx = -1; dy = 0; break;
            case CanonDirection::LEFT: dx = 0; dy = 1; break;
            case CanonDirection::RIGHT: dx = 0; dy = -1; break;
            case CanonDirection::UP_RIGHT: dx = 1; dy = -1; break;
            case CanonDirection::UP_LEFT: dx = 1; dy = 1; break;
            case CanonDirection::DOWN_LEFT : dx = -1; dy = 1; break;
            case CanonDirection::DOWN_RIGHT: dx = -1; dy = -1; break;
            default: break;
        }
    }
    int x_location = (tank_to_move->getLocationX() + dx + height) % height;
    int y_location = (tank_to_move->getLocationY() + dy + width) % width;
    return std::make_pair(x_location, y_location);
}

void GameManager::addTankToLocationMap(std::unordered_map<std::pair<int, int>, std::vector<std::shared_ptr<Tank>>, pair_hash>& map,
    int tank_index, const std::pair<int, int>& cur_location, const std::pair<int, int>& new_location) {

    map[cur_location].push_back(all_tanks[tank_index]);

    if (new_location != cur_location) {
        map[new_location].push_back(all_tanks[tank_index]);
    }
}

void GameManager::checkWallCollision(int tank_index, const std::pair<int,int>& new_location, std::unordered_map<int,bool>& can_tank_move) {
    if (board.isWallLocation(new_location.first, new_location.second)) {
        logger.logInfo("Bad move, Tank " + std::to_string(tank_index) + " hit a wall!");
        can_tank_move[tank_index] = false;
    }
}

void GameManager::checkMineCollision(int tank_index, const std::pair<int,int>& new_location, std::unordered_map<int,bool>& can_tank_move) {
    if (board.isMineLocation(new_location.first, new_location.second)) {
        logger.logInfo("Bad move, Tank " + std::to_string(tank_index) + " hit a mine!");
        can_tank_move[tank_index] = false;
        if (all_tanks[tank_index]->getAlive()) {
            killTank(all_tanks[tank_index]);
        }
        board.setGameObjectAt(new_location.first, new_location.second, std::make_unique<Empty>());
        logger.logInfo("Mine at [" + std::to_string(new_location.first) + ", " + std::to_string(new_location.second) + "] destroyed");
    }
}

void GameManager::checkShellCollision(int tank_index, const std::pair<int,int>& cur_location, const std::pair<int,int>& new_location, std::unordered_map<int,bool>& can_tank_move) {
    for (const auto& shell : flying_shells) {
        bool will_move = can_tank_move[tank_index];
        if ((will_move && (shell->getLocation() == new_location || shell->getPrevLocation() == new_location)) ||
            (!will_move && (shell->getLocation() == cur_location || shell->getPrevLocation() == cur_location))) {
            logger.logInfo("Bad move, shell hit a tank "+ std::to_string(tank_index));
            can_tank_move[tank_index] = false;
            shells_to_delete.insert(shell);
            if (all_tanks[tank_index]->getAlive()) {
                killTank(all_tanks[tank_index]);
            }
        }
    }
}

void GameManager::resolveTankCollisions(const std::unordered_map<std::pair<int, int>, std::vector<std::shared_ptr<Tank>>, pair_hash>& map) {
    for (const auto& [location, tanks] : map) {
        if (tanks.size() > 1) {
            for (const auto& tank : tanks) {
                tanks_to_kill.insert(tank);
            }
        }
    }
}

void GameManager::applyTankKillResults(std::unordered_map<int,bool>& can_tank_move) {
    for (auto& [tank_index, can_move] : can_tank_move) {
        if (tanks_to_kill.find(all_tanks[tank_index]) != tanks_to_kill.end()) {
            can_tank_move[tank_index] = false;
            if (all_tanks[tank_index]->getAlive()) {
                killTank(all_tanks[tank_index]);
            }
            logger.logInfo("Bad move, Tank " + std::to_string(tank_index) + " hit another tank!");
        }
    }
}

std::unordered_map<int,bool> GameManager::checkCollisions(std::unordered_map<int, std::pair<int, int>> new_wanted_locations) {
    std::unordered_map<int,bool> can_tank_move;
    std::unordered_map<std::pair<int, int>, std::vector<std::shared_ptr<Tank>>, pair_hash> tank_locations_map;

    for (auto& [tank_index, new_location] : new_wanted_locations) {
        std::pair<int, int> cur_location = {all_tanks[tank_index]->getLocationX(), all_tanks[tank_index]->getLocationY()};
        can_tank_move[tank_index] = true;

        addTankToLocationMap(tank_locations_map, tank_index, cur_location, new_location);
        checkWallCollision(tank_index, new_location, can_tank_move);
        checkMineCollision(tank_index, new_location, can_tank_move);
        checkShellCollision(tank_index, cur_location, new_location, can_tank_move);
    }

    resolveTankCollisions(tank_locations_map);
    applyTankKillResults(can_tank_move);

    tanks_to_kill.clear();
    return can_tank_move;
}

bool GameManager::handleBackwardWaiting(int tank_index, ActionRequest action) {
    if (action != ActionRequest::MoveBackward && std::get<2>(all_tanks_backwards_info[tank_index])) {
        /* Check if player requested forward while waiting for backward movement, if so, cancel backward waiting.
         * Backward info is: first - counter since requesting backward, 
         * second - if last action preformed is backward movement,
         * third - if player is waiting for backward move. */
        
        if (action == ActionRequest::MoveForward) {
            std::get<2>(all_tanks_backwards_info[tank_index]) = false;
            std::get<0>(all_tanks_backwards_info[tank_index]) = 0;
            logger.logInfo("Backwards waiting for tank " + std::to_string(tank_index) + " cancelled by forward action");
            return false; 
        }
        else {
            std::get<0>(all_tanks_backwards_info[tank_index]) += 1;
            if (std::get<0>(all_tanks_backwards_info[tank_index]) > backward_wating_turns) { 
                std::get<0>(all_tanks_backwards_info[tank_index]) = 0;
                std::get<1>(all_tanks_backwards_info[tank_index]) = false;
                std::get<2>(all_tanks_backwards_info[tank_index]) = false;
                logger.logInfo("Backwards waiting over for tank " + std::to_string(tank_index));
                // Should always move backward
                return false;
            }
            else {
                logger.logInfo("Bad move, action is ignored for tank " + std::to_string(tank_index) + ", still waiting to move backwards");
                return true; 
            }
        }
    }
    return false; 
}

void GameManager::handleBattleInfo(int tank_index) {
    int player_id = all_tanks[tank_index]->getPlayerId();
    auto cur_location = std::make_pair(all_tanks[tank_index]->getLocationX(), all_tanks[tank_index]->getLocationY());
    view->setCharAtLocation(cur_location, called_tank_sign);
    logger.logInfo("Tank " + std::to_string(tank_index) + " requested battle info");
    if (player_id == 1) {
        player1->updateTankWithBattleInfo(all_tanks[tank_index]->getTankAlgorithm(), *view);
        view->setCharAtLocation(cur_location, tank1_sign);
    }
    else {
        player2->updateTankWithBattleInfo(all_tanks[tank_index]->getTankAlgorithm(), *view);
        view->setCharAtLocation(cur_location, tank2_sign);
    }
}

bool GameManager::handleMoveForward(int tank_index, bool can_move, std::pair<int,int> new_location) {
    if (can_move) {
        all_tanks[tank_index]->setLocation(new_location.first, new_location.second);
        logger.logInfo("Tank " + std::to_string(tank_index) + " moved forward to [" + std::to_string(new_location.first) + ", " + std::to_string(new_location.second) + "]");
        return false; 
    }
    return true; 
}

bool GameManager::handleMoveBackward(int tank_index, bool can_move, std::pair<int,int> new_location) {
    if(canMoveBackward(tank_index)) {
        if (can_move) {
            all_tanks[tank_index]->setLocation(new_location.first, new_location.second);
            logger.logInfo("Tank " + std::to_string(tank_index) + " moved backward to [" + std::to_string(new_location.first) + ", " + std::to_string(new_location.second) + "]");
            std::get<0>(all_tanks_backwards_info[tank_index]) = 0;
            std::get<1>(all_tanks_backwards_info[tank_index]) = true;
            std::get<2>(all_tanks_backwards_info[tank_index]) = false;
            return false; 
        }
    }
    else {
        std::get<2>(all_tanks_backwards_info[tank_index]) = true;
        std::get<0>(all_tanks_backwards_info[tank_index]) += 1;
        logger.logInfo("Tank " + std::to_string(tank_index) + " requested backward, need to wait");
    }
    return false;
}

void GameManager::handleRotation(int tank_index, ActionRequest action) {
    CanonDirection current_dir = all_tanks[tank_index]->getCanonDirection();
    switch(action) {
        case ActionRequest::RotateLeft90:
            all_tanks[tank_index]->setCanonDirection(rotate(current_dir, -2));
            break;
        case ActionRequest::RotateRight45:
            all_tanks[tank_index]->setCanonDirection(rotate(current_dir, 1));
            break;
        case ActionRequest::RotateRight90:
            all_tanks[tank_index]->setCanonDirection(rotate(current_dir, 2));
            break;
        case ActionRequest::RotateLeft45:
            all_tanks[tank_index]->setCanonDirection(rotate(current_dir, -1));
            break;
        default:
            break;
    }

    std::ostringstream oss; // We asked ChatGPT to use ostringstream in order to log the rotation action
    oss << "Tank " << tank_index << " asked to rotate to " << action << " and is now rotating to " << all_tanks[tank_index]->getCanonDirection();
    logger.logInfo(oss.str());
}

bool GameManager::handleShooting(int tank_index, const std::unordered_map<int, std::pair<int, int>>& new_wanted_locations) {
    int last = tank_last_shooting[tank_index];
    
    /* Check if allowed to shoot
    *  Since turn counter moves twice as fast as tanks, check for 8 game iterations is equivalent to 4 tank turns and therefore we multiply by 2.
    */
    if(last == -1 || turn_counter - last > shooting_waiting_turns * 2 ) { 
        if(all_tanks[tank_index]->getUnusedShellsCount() > 0){
            auto new_shell_location = getShellLocationOnCreation(all_tanks[tank_index]);
            flying_shells.emplace_back(std::make_shared<Shell>(new_shell_location, all_tanks[tank_index]->getCanonDirection()));
            logger.logInfo("Tank " + std::to_string(tank_index) + " shot a shell at [" + std::to_string(new_shell_location.first) + ", " + std::to_string(new_shell_location.second) + "]");
            updateShellNextLocation(flying_shells.back());
            all_tanks[tank_index]->setUnusedShellsCount(all_tanks[tank_index]->getUnusedShellsCount() - 1);

            // Check if the new shell was created in some tank's new location
            for(auto& [tank_index_1, new_location_1] : new_wanted_locations){
                if (new_shell_location == new_location_1){
                    if(all_tanks[tank_index_1]->getAlive()){
                        logger.logInfo("New shell just hitted tank " + std::to_string(tank_index_1));
                        killTank(all_tanks[tank_index_1]);
                    }

                    // Shell will be deleted next time the function deleteCollidedShells will get called
                    shells_to_delete.insert(flying_shells.back());
                }
            }
            tank_last_shooting[tank_index] = turn_counter;
            return false; 
        }
        else{
            logger.logInfo("Tank " + std::to_string(tank_index) + " has no more shells to shoot");
            return true; 
        }
    }
    else{
        logger.logInfo("Tank " + std::to_string(tank_index) + " is not ready to shoot, needs to wait " + std::to_string(((shooting_waiting_turns * 2) - (turn_counter - last))/2) + " more turns");
        return true; 
    }
}

void GameManager::applyAction(int tank_index, ActionRequest action, bool can_move, std::pair<int, int> new_location, const std::unordered_map<int, std::pair<int, int>>& new_wanted_locations, std::ofstream& output_file) {
    bool is_ignored = false;

    if(handleBackwardWaiting(tank_index, action)){
        is_ignored = true;
    }
    else if (action == ActionRequest::GetBattleInfo){
        handleBattleInfo(tank_index);
    }
    else if (action == ActionRequest::MoveForward){
        is_ignored = handleMoveForward(tank_index, can_move, new_location);
    }
    else if (action == ActionRequest::MoveBackward){
        is_ignored = handleMoveBackward(tank_index, can_move, new_location);
    }
    else if (action == ActionRequest::RotateLeft90 || action == ActionRequest::RotateRight45 ||
             action == ActionRequest::RotateRight90 || action == ActionRequest::RotateLeft45){
        handleRotation(tank_index, action);
    }
    else if (action == ActionRequest::Shoot){
        is_ignored = handleShooting(tank_index, new_wanted_locations);
    }
    else if (action == ActionRequest::DoNothing){
        logger.logInfo("Tank " + std::to_string(tank_index) + " did nothing");
    }
    else {
        logger.logError("Unknown action requested by tank " + std::to_string(tank_index));
        throw std::runtime_error("Unknown action requested by tank " + std::to_string(tank_index));
    }

    // if last action wasn't backward set boolean to false
    if (action != ActionRequest::MoveBackward) std::get<1>(all_tanks_backwards_info[tank_index]) = false;

    if(should_verbose){
        output_file << action;
        if(is_ignored) output_file << " (ignored)";
        if(!all_tanks[tank_index]->getAlive()) output_file << " (killed)";
    }
}

bool GameManager::canMoveBackward(int tank_index) const{
    bool last_action_backward = std::get<1>(all_tanks_backwards_info.at(tank_index));
    int counter = std::get<0>(all_tanks_backwards_info.at(tank_index));
    
    if (last_action_backward || counter == backward_wating_turns) { 
        return true; // Tank can move backward
    } 
    return false;
}

CanonDirection GameManager::rotate(CanonDirection cur_dir, int rotation) {
    // Used ChatGpt to cast the direction to number and preforme rotation as addition/substraction
    int new_dir = static_cast<int>(cur_dir) + rotation;
    if (new_dir < 0) {
        new_dir += 8; // Wrap around to the last direction 
    } else if (new_dir >= 8) {
        new_dir -= 8; // Wrap around to the first direction
    }
    return static_cast<CanonDirection>(new_dir);
}

std::pair<int, int> GameManager::getShellLocationOnCreation(const std::shared_ptr<Tank>& tank_to_shoot) const{
    int dx = 0;
    int dy = 0;
    CanonDirection dir = tank_to_shoot->getCanonDirection();
    /* Used ChatGpt to calculate the adjacent location of tank according to CanonDirection.
     * prompt was "Given the CanonDirection what is the next location" */

    switch(dir){
        case CanonDirection::UP: dx = -1; dy = 0; break;
        case CanonDirection::DOWN: dx = 1; dy = 0; break;
        case CanonDirection::LEFT: dx = 0; dy = -1; break;
        case CanonDirection::RIGHT: dx = 0; dy = 1; break;
        case CanonDirection::UP_RIGHT: dx = -1; dy = 1; break;
        case CanonDirection::UP_LEFT: dx = -1; dy = -1; break;
        case CanonDirection::DOWN_LEFT : dx = 1; dy = -1; break;
        case CanonDirection::DOWN_RIGHT: dx = 1; dy = 1; break;
        default: break;
    }
    int x_location = (tank_to_shoot->getLocationX() + dx + height) % height;
    int y_location = (tank_to_shoot->getLocationY() + dy + width) % width;
    return std::make_pair(x_location, y_location);
}

// Called in every game iteration to delete collided flying shells
void GameManager::deleteCollidedShells(){
    // Used ChatGpt to create and work with set. prompt was "How to create set of pointers of shells, how to add and remove from it"
    for(const std::shared_ptr<Shell>& shell : shells_to_delete){
        flying_shells.erase(std::remove_if(flying_shells.begin(), flying_shells.end(), [&shell](const std::shared_ptr<Shell>& s) { return s == shell; }), flying_shells.end());   
    }
    shells_to_delete.clear();
}

void GameManager::killTank(std::shared_ptr<Tank>& tank_to_kill){
    if(tank_to_kill->getPlayerId() == 1){
        player1_alive_tanks--;
    }
    else{
        player2_alive_tanks--;
    }
    tank_to_kill->setAlive();
}

void GameManager::addTank(int row, int col, int player_id){
    
    if (player_id == 1) {
        all_tanks.emplace_back(std::make_shared<Tank>(row, col, CanonDirection::LEFT, 1, player1_alive_tanks, num_shells));
        player1_alive_tanks++;
    } 
    
    else if (player_id == 2) {
        all_tanks.emplace_back(std::make_shared<Tank>(row, col, CanonDirection::RIGHT, 2, player2_alive_tanks, num_shells));
        player2_alive_tanks++;
    }
}