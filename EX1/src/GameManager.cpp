#include "GameManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>




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

GameManager::GameManager(Player* player1, Player* player2, const std::string& input_file, const std::string& output_file) : player1(player1), player2(player2),tank1(nullptr),tank2(nullptr) ,path_input_file(input_file), path_output_file(output_file), game_over(false), player1_backwards_info(0, false, false), player2_backwards_info(0, false, false), player1_last_shooting(-1), player2_last_shooting(-1), turn_counter(0), no_more_shells(false), counter_no_shells(0) {
    // Initialize the game board and players
    // read first two lines
    std::ifstream file(path_input_file);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << path_input_file << std::endl;
        return;
    }

    std::ostringstream error_log;
    bool has_errors = false;

    std::string line;
    //first row
    if(!std::getline(file, line)){
        std::cerr << "Missing height value in input file.\n";
        throw std::runtime_error("Missing height value in input file.");
    }
    try{
        this->height = std::stoi(line);
    } catch (...){
        std::cerr << "Invalid height value in input file.\n";
        throw std::runtime_error("Invalid height value in input file.");
    }


    //second row
    if(!std::getline(file, line)){
        std::cerr << "Missing width value in input file.\n";
        throw std::runtime_error("Missing width value in input file.");
    }
    try{
        this->width = std::stoi(line);
    } catch (...){
        std::cerr << "Invalid width value in input file.\n";
        throw std::runtime_error("Invalid width value in input file.");
    }

    this->board = Board(height, width); 
    std::cout << "GameManager initialized with board size: " << height << "x" << width << std::endl;

    int row = 0;
    int tank1_count = 0;
    int tank2_count = 0;
    bool missing_col = false;
    bool extra_col = false;

    while(std::getline(file, line)){
        //std::istringstream iss(line);
        if(row >= height){
            has_errors = true;
            error_log << "Too many rows, ignoring.\n";
            break;
        }
        int line_length = static_cast<int>(line.size());
        if(line_length > width){
            extra_col = true;
        }
        for (int col = 0; col < width ; col++){
            char cell;
            if(col < line_length){
                cell = line[col];
            }
            else{
                missing_col = true;
                cell = ' ';
            }
            switch (cell){
                case '#':
                    board.setGameObjectAt(row, col, new Wall());
                    break;
                case '@':
                    board.setGameObjectAt(row, col, new Mine()); // Placeholder for tank direction
                    break;
                case ' ':
                    board.setGameObjectAt(row, col, new Empty());
                    break;
                case '1':{
                    // TODO - should be left, up is for testing
                    if(tank1_count == 0){
                        tank1 = new Tank(row, col, CanonDirection::LEFT); 
                        board.setGameObjectAt(row, col, new Empty());
                        tank1_count++;
                    }
                    else{
                        has_errors = true;
                        error_log << "More than one tank for player 1 at [" << row << ", " << col << "], ignored.\n";
                    }
                    break;
                   
                }
                case '2': {
                    if(tank2_count == 0){
                        tank2 = new Tank(row, col, CanonDirection::RIGHT); 
                        board.setGameObjectAt(row, col, new Empty());
                        tank2_count++;
                    }
                    else{
                        has_errors = true;
                        error_log << "More than one tank for player 2 at [" << row << ", " << col << "], ignored.\n";
                    }
                    break;
                }
                default:
                    board.setGameObjectAt(row, col, new Empty());
                    has_errors = true;
                    error_log << "Unknown character at [" << row << ", " << col << "], treated as space.\n";
                    break;
            }
        }
        row++;
    }
    if(tank1_count == 0){
        std::cerr << "Missing tank for player 1.\n"; 
        throw std::runtime_error("Missing tank for player 1.");
    }
    if(tank2_count == 0){
        std::cerr << "Missing tank for player 2.\n"; 
        throw std::runtime_error("Missing tank for player 2.");
    }
    

    if (missing_col){
        has_errors = true;
        error_log << "Missig column, fiiled with spaces.\n";
    }

    if (extra_col){
        has_errors = true;
        error_log << "Too many columns, ignoring.\n";
    }


    //Fill missing rows if needed
    for(; row < height; ++row){
        for(int col = 0; col < width; ++col){
            board.setGameObjectAt(row, col, new Empty());
        }
        has_errors = true;
        error_log << "Row " << row << " is empty, filled with spaces.\n";
    }

    file.close();

    if(has_errors){
        std::ofstream err_file("input_errors.txt");
        if(err_file.is_open()){
            err_file << error_log.str();
            err_file.close();
        }
    }
}

// void GameManager::prepareGame(){
//     // Read input file
//     std::ifstream file(path_input_file);
//     if (!file.is_open()) {
//         std::cerr << "Error opening file: " << path_input_file << std::endl;
//         return;
//     }

//     std::string line;
//     std::getline(file, line);
//     std::getline(file, line);
//     // Create board
//     int row = 0;
//     while(std::getline(file, line) && row<height){
//         std::istringstream iss(line);
//         int line_length = static_cast<int>(line.size());
//         for (int col = 0; col < width && col<line_length ; ++col){
//             char cell = line[col];
//             switch (cell){
//                 case '#':
//                     board.setGameObjectAt(row, col, new Wall());
//                     break;
//                 case '@':
//                     board.setGameObjectAt(row, col, new Mine()); // Placeholder for tank direction
//                     break;
//                 case ' ':
//                     board.setGameObjectAt(row, col, new Empty());
//                     break;
//                 case '1':{
//                     // TODO - should be left, up is for testing
//                     tank1 = new Tank(row, col, CanonDirection::UP); 
//                     board.setGameObjectAt(row, col, new Empty());
//                     break;
//                 }
//                 case '2': {
//                     tank2 = new Tank(row, col, CanonDirection::RIGHT); 
//                     board.setGameObjectAt(row, col, new Empty());
//                     break;
//                 }
//                 default:
//                     // Handle some error
//                     std::cerr << "Error in board file " << path_input_file << std::endl;
//                     break;

//             }
//         }
//         row++;
//     }

//     file.close();

// }

void GameManager::finishGame(){
    // Write to output file
}


bool GameManager::canMoveBackward(Player* player) const{
    bool last_action_backward = false;
    int counter = 0;
    if(player == player1){
        last_action_backward = std::get<1>(player1_backwards_info);
        counter = std::get<0>(player1_backwards_info);
    }
    else{
        last_action_backward = std::get<1>(player2_backwards_info);
        counter = std::get<0>(player2_backwards_info);
    }
    if (last_action_backward || counter == 2) { 
        return true; // Tank can move backward
    } 
    return false;

}

std::pair<int, int> GameManager::getNewLocation(Player* player, Tank* tank_to_move, Player::Action wanted_action){
    int dx = 0;
    int dy = 0;
    CanonDirection dir = tank_to_move->getCanonDirection();
    if(wanted_action == Player::Action::FORWARD){
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
    else if (wanted_action == Player::Action::BACKWARD && canMoveBackward(player)){
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

std::pair<bool,bool> GameManager::checkCollisions(std::pair<int,int> tank1_location, std::pair<int,int> tank2_location, std::ofstream& output_file){
    // with wall
    bool tank1_can_move = true;
    bool tank2_can_move = true;
    if(board.isWallLocation(tank1_location.first, tank1_location.second)){
        std::cout << "Tank 1 hit a wall!" << std::endl;
        output_file << "Tank 1 hit a wall!" << std::endl;
        tank1_can_move = false; 
    }
    if(board.isWallLocation(tank2_location.first, tank2_location.second)){
        std::cout << "Tank 2 hit a wall!" << std::endl;
        output_file << "Tank 2 hit a wall!" << std::endl;
        tank2_can_move = false;
    }
    // with mine
    if(board.isMineLocation(tank1_location.first, tank1_location.second)){
        std::cout << "Tank 1 hit a mine!" << std::endl;
        output_file << "Tank 1 hit a mine!" << std::endl;
        tank1_can_move = false;
        tank1->setAlive();
    }
    if(board.isMineLocation(tank2_location.first, tank2_location.second)){
        std::cout << "Tank 2 hit a mine!" << std::endl;
        output_file << "Tank 2 hit a mine!" << std::endl;  
        tank2_can_move = false;
        tank2->setAlive();
    }
    // with each other
    if((tank1_location.first == tank2_location.first && tank1_location.second == tank2_location.second) || 
        (tank1_location.first == tank2->getLocationX() && tank1_location.second == tank2->getLocationY() &&
        tank2_location.first == tank1->getLocationX() && tank2_location.second == tank1->getLocationY())){
        std::cout << "Tank 1 and Tank 2 collided!" << std::endl;
        output_file << "Tank 1 and Tank 2 collided!" << std::endl;
        tank1_can_move = false;
        tank2_can_move = false;
        tank1->setAlive();
        tank2->setAlive();
    }
    // with shell
    const std::vector<Shell *>& flying_shells1 = tank1->getFlyingShells();
    const std::vector<Shell *>& flying_shells2 = tank2->getFlyingShells();
    int tank1_hits_counter = 0;
    int tank2_hits_counter = 0;
    for(size_t i = 0; i<flying_shells1.size()+flying_shells2.size(); i++){
        Shell * shell;
        if (i<flying_shells1.size()){
            shell = flying_shells1[i];
        }
        else{
            shell = flying_shells2[i-flying_shells1.size()];
        }
        if (shell->getLocation() == tank1_location || shell->getPrevLocation() == tank1_location){
            shells_to_delete.insert(shell);
            tank1_hits_counter++;
        }
        if (shell->getLocation() == tank2_location || shell->getPrevLocation() == tank2_location){ 
            shells_to_delete.insert(shell);
            tank2_hits_counter++;
        }
    }
    if (tank1_hits_counter > 0){
        tank1_can_move = false;
        tank1->setAlive();
        std::cout << tank1_hits_counter << "shells hit tank 1" << std::endl;
        output_file << tank1_hits_counter << "shells hit tank 1" << std::endl;
    }
    if(tank2_hits_counter > 0){
        tank2_can_move = false;
        tank2->setAlive();
        std::cout << tank2_hits_counter << "shells hit tank 2" << std::endl;
        output_file << tank2_hits_counter << "shells hit tank 2" << std::endl;
    }
    
    return std::make_pair(tank1_can_move, tank2_can_move);

}

bool GameManager::isGameOver(std::ofstream& output_file) {
    // Check if either player is dead
    if (tank1->getAlive() && tank2->getAlive()) {
        if(no_more_shells && counter_no_shells>=80){
            output_file << "Game over - no more shells" << std::endl;
            std::cout << "Game over - no more shells" << std::endl;
            return true;
        }
        return false; 
    }
    if (!tank1->getAlive() && !tank2->getAlive()) {
        output_file << "Tie - both tanks exploded" << std::endl;
        std::cout << "Tie - both tanks exploded" << std::endl;
        return true;
    }
    else if (!tank1->getAlive()) {
        output_file << "Player 2 wins!" << std::endl;
        std::cout << "Player 2 wins!" << std::endl;
        return true;
    } else if (!tank2->getAlive()) {
        output_file << "Player 1 wins!" << std::endl;
        std::cout << "Player 1 wins!" << std::endl;
        return true;
    }
    
    return true;
    
}

CanonDirection GameManager::rotate(CanonDirection cur_dir, int rotation) {
    int new_dir = static_cast<int>(cur_dir) + rotation;
    if (new_dir < 0) {
        new_dir += 8; // Wrap around to the last direction
    } else if (new_dir >= 8) {
        new_dir -= 8; // Wrap around to the first direction
    }
    return static_cast<CanonDirection>(new_dir);
}

std::pair<int, int> GameManager::getShellLocation(const Tank* const tank_to_shoot) const{
    int dx = 0;
    int dy = 0;
    CanonDirection dir = tank_to_shoot->getCanonDirection();
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

void GameManager::MoveShells(bool is_even_turn, std::ofstream& output_file){
    const std::vector<Shell *>& flying_shells1 = tank1->getFlyingShells();
    const std::vector<Shell *>& flying_shells2 = tank2->getFlyingShells();
    std::unordered_map<std::pair<int, int>, std::vector<Shell*>, pair_hash> shell_locations_map;

    for (size_t i = 0; i<flying_shells1.size()+flying_shells2.size(); i++){
        Shell * shell;
        if (i<flying_shells1.size()){
            shell = flying_shells1[i];
        }
        else{
            shell = flying_shells2[i-flying_shells1.size()];
        }
        if (shell_locations_map.find(shell->getLocation()) != shell_locations_map.end()){
            shells_to_delete.insert(shell);
            shells_to_delete.insert(shell_locations_map[shell->getLocation()].begin() ,shell_locations_map[shell->getLocation()].end());
            std::cout << " shells collided" << std::endl;
            output_file << " shells collided" << std::endl;

        }
         
        int x = shell->getNextLocation().first;
        int y = shell->getNextLocation().second;
        if (board.isWallLocation(x, y)) {
            //reduce life for wall and delete shell
            Wall* wall = dynamic_cast<Wall*>(board.getGameObjectAt(x, y));
            wall->reduceLife();
            if(wall->isDestroyed()){
                board.setGameObjectAt(x, y, new Empty());
                std::cout << "Wall destroyed" << std::endl;
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

    if(!is_even_turn){
        int tank1_hits_counter = 0;
        int tank2_hits_counter = 0;
        for (auto& [key, vec] : shell_locations_map){
            if (key.first == tank1->getLocationX() && key.second == tank1->getLocationY())
            {
                tank1_hits_counter++;

            }
            if (key.first == tank2->getLocationX() && key.second == tank2->getLocationY())
            {
                tank2_hits_counter++;
            }
            
        }  
        if (tank1_hits_counter>0){
            tank1 ->setAlive();
            std::cout << tank1_hits_counter << " shells hit tank 1" << std::endl;
            output_file << tank1_hits_counter << " shells hit tank 1" << std::endl;
        }
        if(tank2_hits_counter>0){
            tank2 ->setAlive();
            std::cout << tank2_hits_counter << " shells hit tank 2" << std::endl;
            output_file << tank2_hits_counter << " shells hit tank 2" << std::endl;
        }
    }

    for(auto& [key, vec] : shell_locations_map){
        if (vec.size() > 1)
        {
            std::cout << " shells collided" << std::endl;
            output_file << " shells collided" << std::endl;
            for(Shell * shell: vec){
                shells_to_delete.insert(shell);
            }
        }
        
    } 
   
}

void GameManager::deleteShells(){
    for(Shell * shell : shells_to_delete){
        if(shell->getTankID() == 1){
           tank1->deleteShell(shell);
        }
        else{
            tank2->deleteShell(shell);
        }    
    }
    shells_to_delete.clear();
}


void GameManager::updateShellNextLocation(Shell &shell){
    int dx = 0;
    int dy = 0;
    CanonDirection dir = shell.getFlyingDirection();
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
    int x_location = (shell.getLocation().first + dx + height) % height;
    int y_location = (shell.getLocation().second + dy + width) % width;
    shell.setNextLocation(std::make_pair(x_location, y_location));
}

void GameManager::applyAction(Player* player, Tank* tank_to_aplly, Player::Action action, bool can_move, std::pair<int, int> new_location, std::ofstream& output_file) {
    if (action != Player::Action::BACKWARD){
        if(player == player1 && std::get<2>(player1_backwards_info) ){
            if (action == Player::Action::FORWARD ){
                std::get<2>(player1_backwards_info) = false;
                std::get<0>(player1_backwards_info) = 0;
                output_file << "Tank 1 cancelled backward by moving forward" << std::endl;
                std::cout << "Tank 1 cancelled backward by moving forward" << std::endl;
            }
            else{
                std::get<0>(player1_backwards_info) += 1;
                output_file << "Action is ignored for tank 1, still waiting to move backwards" << std::endl;
                std::cout << "Action is ignored for tank 1, still waiting to move backwards" << std::endl;
            }
            return;
        }
        else if(player == player2 && std::get<2>(player2_backwards_info) ){
            if (action == Player::Action::FORWARD ){
                std::get<2>(player2_backwards_info) = false;
                std::get<0>(player2_backwards_info) = 0;
                output_file << "Tank 2 cancelled backward by moving forward" << std::endl;
                std::cout << "Tank 2 cancelled backward by moving forward" << std::endl;
            }
            else{
                std::get<0>(player2_backwards_info) += 1;
                output_file << "Action is ignored for tank 2, still waiting to move backwards" << std::endl;
                std::cout << "Action is ignored for tank 2, still waiting to move backwards" << std::endl;
            }
            return;
        }
        
    }
    if (action == Player::Action::FORWARD) {
        if (can_move) {
            tank_to_aplly->setLocation(new_location.first, new_location.second);
            output_file << "Tank" << player->getId() << "moved to [" << new_location.first << ", " << new_location.second << "]" << std::endl;
            std::cout << "Tank" << player->getId() << "moved to [" << new_location.first << ", " << new_location.second << "]" << std::endl; 
        }
    } 
    else if (action == Player::Action::BACKWARD ) {
        
        if(canMoveBackward(player)) {
            if (can_move) {
                tank_to_aplly->setLocation(new_location.first, new_location.second);
                output_file << "Tank" << player->getId() << "moved to [" << new_location.first << ", " << new_location.second << "]" << std::endl;
                std::cout << "Tank" << player->getId() << "moved to [" << new_location.first << ", " << new_location.second << "]" << std::endl;
                if (player == player1) {
                    std::get<0>(player1_backwards_info) = 0;
                    std::get<1>(player1_backwards_info) = true;
                    std::get<2>(player1_backwards_info) = false;
                } else {
                    std::get<0>(player2_backwards_info) = 0;
                    std::get<1>(player2_backwards_info) = true;
                    std::get<2>(player2_backwards_info) = false;
                }
            } 
        }
        else{
            // wait
            output_file << "Tank" << player->getId() << "is waiting for backward" << std::endl;
            std::cout << "Tank" << player->getId() << "is waiting for backward"  << std::endl;
            if (player == player1) {
                std::get<2>(player1_backwards_info) = true;
                std::get<0>(player1_backwards_info) += 1;
            } else {
                std::get<2>(player2_backwards_info) = true;
                std::get<0>(player2_backwards_info) += 1;
            }
        }
        
    } else if (action == Player::Action::ROTATE_025_LEFT) {
        // Rotate left logic
        tank_to_aplly->setCanonDirection(rotate(tank_to_aplly->getCanonDirection(), -2));
        std::cout << "Tank " << player->getId() << " rotated 1/4 left to " << tank_to_aplly->getCanonDirection() << std::endl;
        output_file << "Tank " << player->getId() << " rotated 1/4 left to " << tank_to_aplly->getCanonDirection() << std::endl;

    } else if (action == Player::Action::ROTATE_0125_RIGHT) {
        // Rotate right logic
        tank_to_aplly->setCanonDirection(rotate(tank_to_aplly->getCanonDirection(), 1));
        std::cout << "Tank " << player->getId() << " rotated 1/8 right to " << tank_to_aplly->getCanonDirection()<< std::endl;
        output_file << "Tank " << player->getId() << " rotated 1/8 right to " << tank_to_aplly->getCanonDirection()<< std::endl;
    } else if (action == Player::Action::ROTATE_025_RIGHT) {
        // Rotate right logic
        tank_to_aplly->setCanonDirection(rotate(tank_to_aplly->getCanonDirection(), 2));
        std::cout << "Tank " << player->getId() << " rotated 1/4 right to " << tank_to_aplly->getCanonDirection()<< std::endl;
        output_file << "Tank " << player->getId() << " rotated 1/4 right to " << tank_to_aplly->getCanonDirection()<< std::endl;
    } else if (action == Player::Action::ROTATE_0125_LEFT) {
        // Rotate left logic
        tank_to_aplly->setCanonDirection(rotate(tank_to_aplly->getCanonDirection(), -1));
        std::cout << "Tank " << player->getId() << " rotated 1/8 left to " << tank_to_aplly->getCanonDirection() << std::endl;
        output_file << "Tank " << player->getId() << " rotated 1/8 left to " << tank_to_aplly->getCanonDirection() << std::endl;


    } else if (action == Player::Action::SHOOT) {
        // Shoot logic
        int last = -1;
        int id = -1;
        if (player == player1){
            last = player1_last_shooting;
            id = 1;
        }
        else{
            last = player2_last_shooting;
            id = 2;
        }
        
        if(last == -1 || turn_counter- last > 8 ){ 
            if(tank_to_aplly->getUnusedShellsCount() > 0){
                //shoot
                std::pair<int, int> new_shell_location = getShellLocation(tank_to_aplly);
                Shell * shell_to_shoot = new Shell(new_shell_location, tank_to_aplly->getCanonDirection(), id);
                updateShellNextLocation(*shell_to_shoot);
                tank_to_aplly->addFlyingShell(shell_to_shoot);
                tank_to_aplly->setUnusedShellsCount(tank_to_aplly->getUnusedShellsCount() - 1);
                std::cout << "Tank " << player->getId() << " shoot shell"  << std::endl;
                output_file << "Tank " << player->getId() << " shoot shell"  << std::endl;
                if (player == player1) {
                    player1_last_shooting = turn_counter;
                } else {
                    player2_last_shooting = turn_counter;
                }
            }
            else{
                std::cout << "Tank " << player->getId() << " has no more shells to shoot"  << std::endl;
                output_file << "Tank " << player->getId() << " has no more shells to shoot"  << std::endl;
            }
        }
        else{
            std::cout << "Tank " << player->getId() << " is not ready to shoot, needs to wait "  << (8 - turn_counter - last)/2 << " more turns"<< std::endl;
            output_file<< "Tank " << player->getId() << " is not ready to shoot, needs to wait "  << (8 - turn_counter - last)/2 << " more turns"<< std::endl;
        }

    }

    // if last action wasn't backward set boolean to false
    if (action != Player::Action::BACKWARD) {
        if (player == player1) {
            std::get<1>(player1_backwards_info) = false;
        } else {
            std::get<1>(player2_backwards_info) = false;
        }
    }
    

}


void GameManager::runGame(){
    //prepareGame();
    // create and open output file
    std::ofstream output_file;
    output_file.open(path_output_file, std::ios::out);
    if(!output_file.is_open()){
        std::cerr << "Error opening output file: " << path_output_file << std::endl;
        return;
    }

    // Main loop
    while (!isGameOver(output_file)) {
        // Both players and shells should move
        if(turn_counter%2 == 0){
            MoveShells(true, output_file);
            Player::Action action1 = player1->getAction(board, *tank1, *tank2, (player1_last_shooting == -1 || turn_counter- player1_last_shooting > 8));
            std::pair<int, int> new_tank1_Location = getNewLocation(player1, tank1, action1);
            Player::Action action2 = player2->getAction(board, *tank2, *tank1, (player1_last_shooting == -1 || turn_counter- player1_last_shooting > 8));
            std::pair<int, int> new_tank2_Location = getNewLocation(player2, tank2, action2);
            // check collision should check if a tank wants to move to a new shell location or to a new next shell location
            std::pair<bool, bool> can_move = checkCollisions(new_tank1_Location, new_tank2_Location, output_file);
            deleteShells();
            // Move tanks
            applyAction(player1, tank1, action1, can_move.first, new_tank1_Location, output_file);
            applyAction(player2, tank2 ,action2, can_move.second, new_tank2_Location, output_file);
        }
        //Only Shells move
        else{
            MoveShells(false, output_file);
            deleteShells();
        }
        
        if(tank1->getUnusedShellsCount()==0 && tank2->getUnusedShellsCount()==0){
            no_more_shells = true;
            counter_no_shells++;
            std::cout << "Both tanks have no more shells, counting 40 turns to finish"<< std::endl;
            output_file<< "Both tanks have no more shells, counting 40 turns to finish"<<  std::endl;
            std::cout << "counter is "<< counter_no_shells << std::endl;

        }
        turn_counter++;
    }
    finishGame();
}

void GameManager::printBoard(){
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            GameObject* obj = board.getGameObjectAt(i, j);
            if (dynamic_cast<Wall*>(obj)) {
                std::cout << "#";
            } else if (dynamic_cast<Mine*>(obj)) {
                std::cout << "@";
            } else {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}

GameManager::~GameManager() {
    // Clean up resources
    delete player1;
    delete player2;
    //delete tank1;
    //delete tank2;
}