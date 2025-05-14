#include "GameManager.h"


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

GameManager::GameManager(std::unique_ptr<PlayerFactory> player_factory, std::unique_ptr<TankAlgorithmFactory> tank_algorithm_factory) 
    : player_factory(std::move(player_factory)),
      tank_algorithm_factory(std::move(tank_algorithm_factory)),
      turn_counter(0), 
      no_more_shells(false), 
      counter_no_shells(0),
      //TODO - we are worried that board is calling copy constractor, check it.
      view(SatelliteViewImp(height, width, board, flying_shells, all_tanks)){}   


void GameManager::readBoard(const std::string& path_input_file) {
    size_t last_slash = path_input_file.find_last_of("/\\");
    const std::string input_file_name = (last_slash == std::string::npos) ? path_input_file : path_input_file.substr(last_slash + 1);
    this->path_output_file = "output/output_"+input_file_name;

    std::ifstream file(path_input_file);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << path_input_file << std::endl;
        return;
    }

    std::ostringstream error_log;
    bool has_errors = false;

    std::string line;

    // Line 1 - Description (ignored)
    std::getline(file, line);

    // Line 2 - MaxSteps
    if (!std::getline(file, line)) {
        throw std::runtime_error("Missing MaxSteps line.");
    }
    size_t pos = line.find("=");
    if (pos == std::string::npos) {
        throw std::runtime_error("Invalid MaxSteps line.");
    }
    try {
        this->max_steps = std::stoi(line.substr(pos + 1));
    } catch (...) {
        throw std::runtime_error("Invalid MaxSteps value.");
    }

    // Line 3 - NumShells
    if (!std::getline(file, line)) {
        throw std::runtime_error("Missing NumShells line.");
    }
    pos = line.find("=");
    if (pos == std::string::npos) {
        throw std::runtime_error("Invalid NumShells line.");
    }
    try {
        this->num_shells = std::stoi(line.substr(pos + 1));
    } catch (...) {
        throw std::runtime_error("Invalid NumShells value.");
    }

    // Line 4 - Rows
    if (!std::getline(file, line)) {
        throw std::runtime_error("Missing Rows line.");
    }
    pos = line.find("=");
    if (pos == std::string::npos) {
        throw std::runtime_error("Invalid Rows line.");
    }
    try {
        this->height = std::stoi(line.substr(pos + 1));
    } catch (...) {
        throw std::runtime_error("Invalid Rows value.");
    }

    // Line 5 - Cols
    if (!std::getline(file, line)) {
        throw std::runtime_error("Missing Cols line.");
    }
    pos = line.find("=");
    if (pos == std::string::npos) {
        throw std::runtime_error("Invalid Cols line.");
    }
    try {
        this->width = std::stoi(line.substr(pos + 1));
    } catch (...) {
        throw std::runtime_error("Invalid Cols value.");
    }

    this->board = Board(height, width);
    std::cout << "GameManager initialized with board size: " << height << "x" << width << std::endl;

    int row = 0;
    bool missing_col = false;
    bool extra_col = false;

    while (std::getline(file, line)) {
        if (row >= height) {
            has_errors = true;
            error_log << "Too many rows, ignoring line " << row << ".\n";
            continue;
        }

        int line_length = static_cast<int>(line.size());
        if (line_length > width) {
            extra_col = true;
        }

        for (int col = 0; col < width; ++col) {
            char cell = (col < line_length) ? line[col] : ' ';
            if (col >= line_length) {
                missing_col = true;
            }

            switch (cell) {
                case '#':
                    board.setGameObjectAt(row, col, std::move(std::make_unique<Wall>()));
                    break;
                case '@':
                    board.setGameObjectAt(row, col, std::move(std::make_unique<Mine>()));
                    break;
                case ' ':
                    board.setGameObjectAt(row, col, std::move(std::make_unique<Empty>()));
                    break;
                case '1':
                    // Add a new tank for player 1
                    all_tanks.emplace_back(row, col, CanonDirection::LEFT, 1, player1_alive_tanks, num_shells, tank_algorithm_factory->create(1, player1_alive_tanks));
                    player1_alive_tanks ++;
                    board.setGameObjectAt(row, col, std::move(std::make_unique<Empty>()));
                    break;
                case '2':
                    // Add a new tank for player 2
                    all_tanks.emplace_back(row, col, CanonDirection::RIGHT, 2, player2_alive_tanks, num_shells ,tank_algorithm_factory->create(2, player2_alive_tanks));
                    player2_alive_tanks ++;
                    board.setGameObjectAt(row, col, std::move(std::make_unique<Empty>()));
                    break;
                default:
                    board.setGameObjectAt(row, col, std::move(std::make_unique<Empty>()));
                    has_errors = true;
                    error_log << "Unknown character '" << cell << "' at [" << row << ", " << col << "], treated as space.\n";
                    break;
            }
        }

        ++row;
    }

    // Fill missing rows
    for (; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            board.setGameObjectAt(row, col, std::make_unique<Empty>());
        }
        has_errors = true;
        error_log << "Row " << row << " is missing, filled with spaces.\n";
    }

    file.close();

    if (player1_alive_tanks == 0) {
        throw std::runtime_error("No tanks for player 1.");
    }
    if (player2_alive_tanks == 0) {
        throw std::runtime_error("No tanks for player 2.");
    }

    if (missing_col) {
        error_log << "Some rows had missing columns, filled with spaces.\n";
    }
    if (extra_col) {
        error_log << "Some rows had too many columns, ignored extra characters.\n";
    }

    if (has_errors) {
        std::ofstream err_file("input_errors.txt");
        if (err_file.is_open()) {
            err_file << error_log.str();
            err_file.close();
        }
    }
}

void GameManager::run(){
    //create players
    this->player1 = player_factory->create(1, width, height, max_steps, num_shells);
    this->player2 = player_factory->create(2, width, height, max_steps, num_shells);

    // initialize dicts
    for(size_t i=0; i<all_tanks.size(); i++){
        tank_last_shooting[i] = -1;
        all_tanks_backwards_info[i] = std::make_tuple(0, false, false);
    }

    // create and open output file
    std::ofstream output_file;
    output_file.open(path_output_file, std::ios::out);
    if(!output_file.is_open()){
        std::cerr << "Error opening output file: " << path_output_file << std::endl;
        return;
    }

    // Only for live tanks
    std::unordered_map<int,ActionRequest> wanted_actions;
    std::unordered_map<int, std::pair<int, int>> new_wanted_locations;

    // Main loop
    while (!isGameOver(output_file)) {
        std::cout << "Turn is "<< turn_counter << std::endl;
        output_file << "Turn is "<< turn_counter <<  std::endl;
        // Even turn - both players and shells should move
        if(turn_counter%2 == 0){
            MoveShells(true, output_file);
            
            // Request action from all live tanks using their algorithms
            for(size_t i = 0; i<all_tanks.size(); i++){
                if(all_tanks[i].getAlive()){
                    wanted_actions[i] =  all_tanks[i].getTankAlgorithm().getAction();
                    new_wanted_locations[i] = getNewLocation(all_tanks[i], wanted_actions[i]);
                }
            }


            // check collision should check if a tank wants to move to a new shell location or to a new next shell location
            std::unordered_map<int,bool> can_move = checkCollisions(new_wanted_locations, output_file);
            deleteCollidedShells();

            // Applying actions
            for(size_t i = 0; i<all_tanks.size(); i++){
                //check tank index not in can_move, tank was already dead in begining of this turn so there is nothing to do
                if(can_move.find(i) == can_move.end()){
                    output_file << "killed";
                }
                else{
                    applyAction(i, wanted_actions[i], can_move[i], new_wanted_locations[i], new_wanted_locations, output_file);
                }
                if (i!=all_tanks.size()-1){
                    output_file << ", ";
                }
            }   
            // To delete shells that were created exactly on a tank, so on creation they explode the tank and themselves.
            deleteCollidedShells();


        }
        //Odd turn - only Shells move
        else{
            std::cout << "Odd turn - only flying shells move."<< std::endl;
            MoveShells(false, output_file);
            deleteCollidedShells();
        }
        
        // Check if all shells were shooted and should start counting for finish.
        if(shellFinished()){
            no_more_shells = true;
            counter_no_shells++;
            std::cout << "Both players have no more shells, counting 40 turns to finish"<< std::endl;
            std::cout << "counter is "<< counter_no_shells << std::endl;
        }
        turn_counter++;
    }
}

// Check if both players finished their shells for all tanks
bool GameManager::shellFinished(){
    for(size_t i=0; i<all_tanks.size(); i++){
        if(all_tanks[i].getUnusedShellsCount() > 0){
            return false;
        }
    }
    return true;
}

bool GameManager::isGameOver(std::ofstream& output_file) {
    // Check if both players finished their shells and 40 turns passed
    if (player1_alive_tanks > 0  && player2_alive_tanks > 0) {
        if(no_more_shells && counter_no_shells>=80){
            output_file << "Game over - no more shells" << std::endl;
            std::cout << "Game over - no more shells" << std::endl;
            return true;
        }
        if(turn_counter >= max_steps*2){
            output_file << "Game over - max steps reached" << std::endl;
            std::cout << "Game over - max steps reached" << std::endl;
            return true;
        }
        return false; 
    }
    // Both players are dead
    if (player1_alive_tanks == 0  && player2_alive_tanks == 0) {
        output_file << "Tie - both players tanks exploded" << std::endl;
        std::cout << "Tie - both players tanks exploded" << std::endl;
        return true;
    }
    // Only player1 dead
    else if (player1_alive_tanks == 0) {
        output_file << "Player 2 wins!" << std::endl;
        std::cout << "Player 2 wins!" << std::endl;
        return true;
    // Only player 2 dead
    } else if (player2_alive_tanks == 0) {
        output_file << "Player 1 wins!" << std::endl;
        std::cout << "Player 1 wins!" << std::endl;
        return true;
    }
    
    return true;  
}

void GameManager::MoveShells(bool is_even_turn, std::ofstream& output_file){

    // Map betwenn location and the shells that will arrive to this new location
    // Used ChatGpt to learn to work with dictonaries, prompt was - "How to create a dict in c++ to map between pair to list of pointers?"
    std::unordered_map<std::pair<int, int>, std::vector<Shell*>, pair_hash> shell_locations_map;

    for (size_t i = 0; i<flying_shells.size(); i++){
        Shell* shell = &flying_shells[i];
        // If shell is in a location that is a new location for another shell - collision
        std::pair <int, int> shell_cur_location = shell->getLocation(); 
        if (shell_locations_map.find(shell_cur_location) != shell_locations_map.end()){
            shells_to_delete.insert(shell);
            shells_to_delete.insert(shell_locations_map[shell_cur_location].begin() ,shell_locations_map[shell_cur_location].end());
            std::cout << "shells are colliding at location [" << shell_cur_location.first <<", "<< shell_cur_location.second << "]"<< std::endl;
            output_file << "shells are colliding at location [" << shell_cur_location.first <<", "<< shell_cur_location.second << "]" << std::endl;
        }
         
        int x = shell->getNextLocation().first;
        int y = shell->getNextLocation().second;
        // Check if shell is hitting a wall
        if (board.isWallLocation(x, y)) {
            //reduce life for wall and delete shell if needed
            Wall* wall = dynamic_cast<Wall*>(board.getGameObjectAt(x, y));
            wall->reduceLife();
            if(wall->isDestroyed()){
                board.setGameObjectAt(x, y, std::make_unique<Empty>());
                std::cout << "Wall at ["<< x << ", "<< y << "] destroyed" << std::endl;
                delete wall;
            }
            //remove shell from flying_shells1
            shells_to_delete.insert(shell);
            continue;
        }
        // move
        shell->setPrevLocation(shell->getLocation());
        shell->setLocation(shell->getNextLocation());
        updateShellNextLocation(*shell);
        shell_locations_map[shell->getLocation()].push_back(shell); // add shell to the map
    }

    // On odd turns only shells move, so should check if they are hitting a tank, 
    //on even turn, tank can also move so there is more complex check in checkCollisions.
    if(!is_even_turn){
        for (auto& [key, vec] : shell_locations_map){
            
            for (size_t i = 0; i< all_tanks.size(); i++){
                if (key.first == all_tanks[i].getLocationX() && key.second == all_tanks[i].getLocationY()){
                    if(all_tanks[i].getPlayerId() == 1){
                        player1_alive_tanks--;
                    }
                    else{
                        player2_alive_tanks--;
                    }
                    all_tanks[i].setAlive();
                }
            }
            
        }  
        
    }
    // If multiple shells arrive to the same location - collision between all of them
    // Used ChatGpt to iterate over the dictonary - prompt was "How to iterate over the dict?"
    for(auto& [key, vec] : shell_locations_map){
        if (vec.size() > 1)
        {
            std::cout << vec.size() << " shells collided at location [" << key.first <<", "<< key.second << "]" << std::endl;
            output_file << vec.size() << " shells collided at location [" << key.first <<", "<< key.second << "]" << std::endl;
            for(Shell * shell: vec){
                shells_to_delete.insert(shell);
            }
        }  
    } 
}

void GameManager::updateShellNextLocation(Shell &shell){
    int dx = 0;
    int dy = 0;
    CanonDirection dir = shell.getFlyingDirection();
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
    int x_location = (shell.getLocation().first + dx + height) % height;
    int y_location = (shell.getLocation().second + dy + width) % width;
    shell.setNextLocation(std::make_pair(x_location, y_location));
}

std::pair<int, int> GameManager::getNewLocation(const Tank& tank_to_move, ActionRequest wanted_action){
    int dx = 0;
    int dy = 0;
    CanonDirection dir = tank_to_move.getCanonDirection();
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
    else if (wanted_action == ActionRequest::MoveBackward && canMoveBackward(tank_to_move.getPlayerId())){
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
    int x_location = (tank_to_move.getLocationX() + dx + height) % height;
    int y_location = (tank_to_move.getLocationY() + dy + width) % width;
    return std::make_pair(x_location, y_location);
}

std::unordered_map<int,bool> GameManager::checkCollisions(std::unordered_map<int, std::pair<int, int>> new_wanted_locations, std::ofstream& output_file){
    std::unordered_map<int,bool> can_tank_move;

    // Map betwenn location and the tanks that will arrive to this new location
    // Used ChatGpt to learn to work with dictonaries, prompt was - "How to create a dict in c++ to map between pair to list of pointers?"
    std::unordered_map<std::pair<int, int>, std::vector<Tank*>, pair_hash> tank_locations_map;

    for(auto& [tank_index, new_location] : new_wanted_locations){

        std::pair<int, int> cur_location = std::make_pair(all_tanks[tank_index].getLocationX(), all_tanks[tank_index].getLocationY());
        Tank* tank = &all_tanks[tank_index];

        can_tank_move[tank_index] = true;

        tank_locations_map[cur_location].push_back(tank);
        tank_locations_map[new_location].push_back(tank);

        // With wall
        if(board.isWallLocation(new_location.first, new_location.second)){
            std::cout << "Bad move, Tank " << tank_index << " hit a wall!" << std::endl;
            output_file << "Bad move, Tank " << tank_index << " hit a wall!" << std::endl;
            can_tank_move[tank_index] = false;
        }
        // With mine
        if(board.isMineLocation(new_location.first, new_location.second)){
            std::cout << "Bad move, Tank " << tank_index << " hit a mine!" << std::endl;
            output_file << "Bad move, Tank " << tank_index << " hit a mine!" << std::endl;
            can_tank_move[tank_index] = false;
            all_tanks[tank_index].setAlive();
            // TODO - delete mine so other tanks in the future will not hit it
        }
        for(size_t j = 0; j<flying_shells.size(); j++){
            Shell* shell = &flying_shells[j];

            /* Since shells flies twice as fast as tank, in every tank move we check collision with the shell current location and also shell prev location
            * Example for a tank turn:
            * Shell moves from [0,0] to [0,2]
            * Tank moves from [1,1] to [0,1]
            * new shell location after this move should be [0,2], but there is a collision in [1,1], so check for also prev location to identify it. */
            if(shell->getLocation() == new_location || shell->getPrevLocation() == new_location){
                std::cout << "Bad move, Tank " << tank_index << " hit a shell!" << std::endl;
                output_file << "Bad move, Tank " << tank_index << " hit a shell!" << std::endl;
                can_tank_move[tank_index] = false;
                shells_to_delete.insert(shell);
                all_tanks[tank_index].setAlive();
            }
        }   

    }
    // If multiple shells arrive to the same location - collision between all of them
    // Used ChatGpt to iterate over the dictonary - prompt was "How to iterate over the dict?"
    for(auto& [key, vec] : tank_locations_map){
        if (vec.size() > 1)
        {
            for(Tank * tank: vec){
                tanks_to_kill.insert(tank);
            }
        }  
    }

    // iterate over can_tank_move and check if the tank is in the tanks_to_kill set
    for(auto& [tank_index, can_move] : can_tank_move){
        if(tanks_to_kill.find(&all_tanks[tank_index]) != tanks_to_kill.end()){
            can_tank_move[tank_index] = false;
            all_tanks[tank_index].setAlive();
            std::cout << "Bad move, Tank " << tank_index << " hit another tank!" << std::endl;
            output_file << "Bad move, Tank " << tank_index << " hit another tank!" << std::endl;
        }
    }
    tanks_to_kill.clear();
   
    return can_tank_move;
}

void GameManager::applyAction(int tank_index, ActionRequest action, bool can_move, std::pair<int, int> new_location, const std::unordered_map<int, std::pair<int, int>>& new_wanted_locations, std::ofstream& output_file) {

    if(!all_tanks[tank_index].getAlive()){
        output_file << action << " (killed)";
        return;
    }

    if (action != ActionRequest::MoveBackward && std::get<2>(all_tanks_backwards_info[tank_index])){
        /* Check if player requested forward while waiting for backward movement, if so, cancel backward waiting.
         * Backward info is: first - counter since requesting backward, 
         * second - if last action preformed is backward movement,
         * third - if player is waiting for backward move. */
        
        if (action == ActionRequest::MoveForward ){
            std::get<2>(all_tanks_backwards_info[tank_index]) = false;
            std::get<0>(all_tanks_backwards_info[tank_index]) = 0;
            output_file << "Tank 1 cancelled backward by moving forward" << std::endl;
            std::cout << "Tank 1 cancelled backward by moving forward" << std::endl;
        }
        else{
            std::get<0>(all_tanks_backwards_info[tank_index]) += 1;
            if (std::get<0>(all_tanks_backwards_info[tank_index]) > 2){
                std::get<0>(all_tanks_backwards_info[tank_index]) = 0;
                std::get<1>(all_tanks_backwards_info[tank_index]) = false;
                std::get<2>(all_tanks_backwards_info[tank_index]) = false;   
                output_file << "Backwards waiting over for tank 1" << std::endl;
                std::cout << "Backwards waiting over for tank 1" << std::endl;
            }
            else{
                output_file << "Bad move, action is ignored for tank 1, still waiting to move backwards" << std::endl;
                std::cout << "Bad, move, action is ignored for tank 1, still waiting to move backwards" << std::endl;
            }
        }
       
    }
    else if(action == ActionRequest::GetBattleInfo){
        int player_id = all_tanks[tank_index].getPlayerId();
        view.setCalledPlayerId(player_id);
        view.setCalledTankIndex(all_tanks[tank_index].getTankIndex());
        if(player_id == 1){
            player1->updateTankWithBattleInfo(all_tanks[tank_index].getTankAlgorithm(), view);
        }
        else{
            player2->updateTankWithBattleInfo(all_tanks[tank_index].getTankAlgorithm(), view);
        }

    }

    else if (action == ActionRequest::MoveForward) {
        if (can_move) {
            all_tanks[tank_index].setLocation(new_location.first, new_location.second);
        }
    } 

    else if (action == ActionRequest::MoveBackward ) {
        if(canMoveBackward(tank_index)) {
            if (can_move) {
                all_tanks[tank_index].setLocation(new_location.first, new_location.second);
                std::get<0>(all_tanks_backwards_info[tank_index]) = 0;
                std::get<1>(all_tanks_backwards_info[tank_index]) = true;
                std::get<2>(all_tanks_backwards_info[tank_index]) = false;   
            } 
        }
        else{
            // wait
            std::get<2>(all_tanks_backwards_info[tank_index]) = true;
            std::get<0>(all_tanks_backwards_info[tank_index]) += 1;
            
        }
        
    } else if (action == ActionRequest::RotateLeft90) {
        // Rotate left logic
        all_tanks[tank_index].setCanonDirection(rotate(all_tanks[tank_index].getCanonDirection(), -2));
        

    } else if (action == ActionRequest::RotateRight45) {
        // Rotate right logic
        all_tanks[tank_index].setCanonDirection(rotate(all_tanks[tank_index].getCanonDirection(), 1));
        
    } else if (action == ActionRequest::RotateRight90) {
        // Rotate right logic
        all_tanks[tank_index].setCanonDirection(rotate(all_tanks[tank_index].getCanonDirection(), 2));
        
    } else if (action == ActionRequest::RotateLeft45) {
        // Rotate left logic
        all_tanks[tank_index].setCanonDirection(rotate(all_tanks[tank_index].getCanonDirection(), -1));


    } else if (action == ActionRequest::Shoot) {
        // Shoot logic
        int last = tank_last_shooting[tank_index];
        // Check if allowed to shoot
        // Since turn counter moves twice as fast as tanks, check for 8 game iterations is equivalent to 4 tank turns.
        if(last == -1 || turn_counter - last > 8 ){ 
            if(all_tanks[tank_index].getUnusedShellsCount() > 0){
                std::pair<int, int> new_shell_location = getShellLocationOnCreation(all_tanks[tank_index]);
                //Shell * shell_to_shoot = new Shell(new_shell_location, tank_to_apply.getCanonDirection(), id);

                flying_shells.emplace_back(new_shell_location, all_tanks[tank_index].getCanonDirection(), tank_index);
                updateShellNextLocation(flying_shells.back());

                all_tanks[tank_index].setUnusedShellsCount(all_tanks[tank_index].getUnusedShellsCount() - 1);
                
                // Check if the new shell was created in some tank's new location
                for(auto& [tank_index, new_location] : new_wanted_locations){
                    if (new_shell_location.first == new_location.first && new_shell_location.second == new_location.second){
                        all_tanks[tank_index].setAlive();
                        if(all_tanks[tank_index].getPlayerId() == 1){
                            player1_alive_tanks--;
                        }
                        else{
                            player2_alive_tanks--;
                        }
                        // Shell will be deleted next time the function deleteCollidedShells will get called
                        shells_to_delete.insert(&flying_shells.back());
                    }
                }
            }
            else{
                std::cout << "Tank " << tank_index << " has no more shells to shoot"  << std::endl;
            }
        }
        else{
            std::cout << "Tank " << tank_index << " is not ready to shoot, needs to wait "  << (8 - (turn_counter - last))/2 << " more turns"<< std::endl;
        }
    }

    // if last action wasn't backward set boolean to false
    if (action != ActionRequest::MoveBackward) {
        std::get<1>(all_tanks_backwards_info[tank_index]) = false;
    }
    output_file << action;
}

bool GameManager::canMoveBackward(int tank_index) const{
    bool last_action_backward = std::get<1>(all_tanks_backwards_info.at(tank_index));
    int counter = std::get<0>(all_tanks_backwards_info.at(tank_index));
    
    if (last_action_backward || counter == 2) { 
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

std::pair<int, int> GameManager::getShellLocationOnCreation(const Tank& tank_to_shoot) const{
    int dx = 0;
    int dy = 0;
    CanonDirection dir = tank_to_shoot.getCanonDirection();
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
    int x_location = (tank_to_shoot.getLocationX() + dx + height) % height;
    int y_location = (tank_to_shoot.getLocationY() + dy + width) % width;
    return std::make_pair(x_location, y_location);
}

// Called in every game iteration to delete collided flying shells
void GameManager::deleteCollidedShells(){
    // Used ChatGpt to create and work with set. prompt was "How to create set of pointers of shells, how to add and remove from it"
    for(Shell * shell : shells_to_delete){
        flying_shells.erase(std::remove_if(flying_shells.begin(), flying_shells.end(), [shell](const Shell& s) { return &s == shell; }), flying_shells.end());   
    }
    shells_to_delete.clear();
}
