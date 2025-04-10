#include "GameManager.h"
#include <fstream>
#include <iostream>
#include <sstream>

GameManager::GameManager(Player* player1, Player* player2, const std::string& input_file, const std::string& output_file) : player1(player1), player2(player2), path_input_file(input_file), path_output_file(output_file), game_over(false) {
    // Initialize the game board and players
    // read first two lines
    std::ifstream file(path_input_file);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << path_input_file << std::endl;
        return;
    }

    std::string line;
    //first row
    std::getline(file, line);
    this->width = std::stoi(line);
    //second row
    std::getline(file, line);
    this->height = std::stoi(line);

    this->board = Board(height, width); 
    std::cout << "GameManager initialized with board size: " << height << "x" << width << std::endl;
    file.close();
}

void GameManager::prepareGame(){
    // Read input file
    std::ifstream file(path_input_file);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << path_input_file << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line);
    std::getline(file, line);
    // Create board
    int row = 0;
    while(std::getline(file, line) && row<height){
        std::istringstream iss(line);
        int line_length = static_cast<int>(line.size());
        for (int col = 0; col < width && col<line_length ; ++col){
            char cell = line[col];
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
                    Tank* first_tank = new Tank(row, col, CanonDirection::LEFT); 
                    player1->setTank(first_tank);
                    board.setGameObjectAt(row, col, new Empty());
                    break;
                }
                case '2': {
                    Tank* second_tank = new Tank(row, col, CanonDirection::RIGHT); 
                    player2->setTank(second_tank);
                    board.setGameObjectAt(row, col, new Empty());
                    break;
                }
                default:
                    // Handle some error
                    std::cerr << "Error in board file " << path_input_file << std::endl;
                    break;

            }
        }
        row++;
    }

    file.close();

}

void GameManager::finishGame(){
    // Write to output file
    // Clean up resources
}

std::pair<int, int> GameManager::getNewLocation(const Tank& tank_to_move, Player::Action wanted_action){
    int dx = 0;
    int dy = 0;
    CanonDirection dir = tank_to_move.getCanonDirection();
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
    else if (wanted_action == Player::Action::BACKWARD){
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
    int x_location = (tank_to_move.getLocationX() + dx) % height;
    int y_location = tank_to_move.getLocationY() + dy % width;
    return std::make_pair(x_location, y_location);
}

std::pair<bool,bool> GameManager::checkCollisions(Tank tank1, Tank tank2, std::pair<int,int> tank1_location, std::pair<int,int> tank2_location, std::ofstream& output_file){
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
        player1->getTank()->setAlive();
    }
    if(board.isMineLocation(tank2_location.first, tank2_location.second)){
        std::cout << "Tank 2 hit a mine!" << std::endl;
        output_file << "Tank 2 hit a mine!" << std::endl;  
        tank2_can_move = false;
        player2->getTank()->setAlive();
    }
    // with each other
    if((tank1_location.first == tank2_location.first && tank1_location.second == tank2_location.second) || 
        (tank1_location.first == tank2.getLocationX() && tank1_location.second == tank2.getLocationY() &&
        tank2_location.first == tank1.getLocationX() && tank2_location.second == tank1.getLocationY())){
        std::cout << "Tank 1 and Tank 2 collided!" << std::endl;
        output_file << "Tank 1 and Tank 2 collided!" << std::endl;
        tank1_can_move = false;
        tank2_can_move = false;
        player1->getTank()->setAlive();
        player2->getTank()->setAlive();
    }
    return std::make_pair(tank1_can_move, tank2_can_move);

}

bool GameManager::isGameOver(std::ofstream& output_file) {
    // Check if either player is dead
    if (player1->getTank()->getAlive() && player2->getTank()->getAlive()) {
        return false; // Game is not over
    }
    if (!player1->getTank()->getAlive() && !player2->getTank()->getAlive()) {
        output_file << "Tie - both tanks exploded" << std::endl;
        std::cout << "Tie - both tanks exploded" << std::endl;
        return true;
    }
    else if (!player1->getTank()->getAlive()) {
        output_file << "Player 2 wins!" << std::endl;
        std::cout << "Player 2 wins!" << std::endl;
        return true;
    } else if (!player2->getTank()->getAlive()) {
        output_file << "Player 1 wins!" << std::endl;
        std::cout << "Player 1 wins!" << std::endl;
        return true;
    }
    return true;
    
}


void GameManager::runGame(){
    prepareGame();
    // create and open output file
    std::ofstream output_file;
    output_file.open(path_output_file, std::ios::out);
    if(!output_file.is_open()){
        std::cerr << "Error opening output file: " << path_output_file << std::endl;
        return;
    }

    // Main loop
    int turn_counter = 0;
    while (!isGameOver(output_file)) {
        // Both players and shells should move
        // clean up exploded zones in the end
        if(turn_counter%2 == 0){
            // first should move shells
            Player::Action action1 = player1->getAction(board, *player1->getTank(), *player2->getTank());
            std::pair<int, int> new_tank1_Location = getNewLocation(*player1->getTank(), action1);
            Player::Action action2 = player2->getAction(board, *player1->getTank(), *player2->getTank());
            std::pair<int, int> new_tank2_Location = getNewLocation(*player2->getTank(), action2);
            std::pair<bool, bool> can_move = checkCollisions(*player1->getTank(), *player2->getTank(), new_tank1_Location, new_tank2_Location, output_file);
            // Move tanks
            if (can_move.first){
                player1->getTank()->setLocation(new_tank1_Location.first, new_tank1_Location.second);
                output_file << "Tank 1 moved to [" << new_tank1_Location.first << ", " << new_tank1_Location.second << "]" << std::endl;
                std::cout << "Tank 1 moved to [" << new_tank1_Location.first << ", " << new_tank1_Location.second << "]" << std::endl; 
            }
            if(can_move.second){
                player2->getTank()->setLocation(new_tank2_Location.first, new_tank2_Location.second);
                output_file << "Tank 2 moved to [" << new_tank2_Location.first << ", " << new_tank2_Location.second << "]" << std::endl;
                std::cout << "Tank 2 moved to [" << new_tank2_Location.first << ", " << new_tank2_Location.second << "]" << std::endl; 
            }


        }
        // Only Shells move
        // else{
        //     ;
        // }
        
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
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            delete board.getGameObjectAt(i, j);
        }
    }
}