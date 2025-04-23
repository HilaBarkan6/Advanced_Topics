#include "Player.h"
#include <cmath>

int Player::shellIsComming(const Board& board, const Tank& my_tank, const Tank& op_tank, int x_to_check, int y_to_check) const{
    int x = x_to_check;
    int y = y_to_check;
    int width = board.getColumns();
    //int height = board.getRows();
    
    const std::vector<Shell *>& flying_shells1 = op_tank.getFlyingShells();
    const std::vector<Shell *>& flying_shells2 = my_tank.getFlyingShells();
    std::vector<Shell *> all_shells;
    all_shells.insert(all_shells.end(), flying_shells1.begin(), flying_shells1.end());
    all_shells.insert(all_shells.end(), flying_shells2.begin(), flying_shells2.end());

    int min_distance = INT_MAX;
    for(Shell * shell : all_shells){
        std::pair<int,int> shell_prev_location = shell->getPrevLocation();
        /* Player shouldn't move into shell_prev_location, since shells fly twice as fast as player and shells are moved before (according to Amir's recommendation in the forum)
         * Example for a tank turn:
         * Shell moves from [0,0] to [0,2]
         * Player considers to move from [1,1] to [0,1]
         * new shell location after this move should be [0,2], but there is a collision in [1,1], so this is a bad move and should be avoided */
        if(shell_prev_location.first == x && shell_prev_location.second == y){
            return 0;
        }
        std::pair<int, int> shell_location = shell->getLocation();
        int sx = shell_location.first;
        int sy = shell_location.second;
        CanonDirection dir = shell->getFlyingDirection();
        // Used ChatGpt to calculate distances, prompt was "Given the location of the shell and tank, check if according to the shell direction it can hit the tank"
        // We also gave ChatGpt the instructions about shells movement.
        if(sx == x){
            if(dir == CanonDirection::RIGHT){
                if(sy < y){
                    if(clearPath(board, sx, sy, x, y)){
                        min_distance = std::min(min_distance, abs(y-sy));
                    }
                }
                else{
                    if(clearPath(board, sx, sy, x, y)){
                        min_distance = std::min(min_distance, width - abs(y-sy));
                    } 
                }
            }
            
            if(sy > y && dir == CanonDirection::LEFT){
                if(clearPath(board, sx, sy, x, y)){
                    min_distance = std::min(min_distance, abs(y-sy));
                }
            }
        }
        if(sy == y){
            if(sx < x && dir == CanonDirection::DOWN){
                if(clearPath(board, sx, sy, x, y)){
                    min_distance = std::min(min_distance, abs(x-sx));
                }
            }
            if(sx > x && dir == CanonDirection::UP){
                if(clearPath(board, sx, sy, x, y)){
                    min_distance = std::min(min_distance, abs(x-sx));
                }
            }
        }
        int dx = x-sx;
        int dy = y-sy;
        if(abs(dx) == abs(dy)){
            if(dx < 0 && dy > 0 && dir == CanonDirection::UP_RIGHT){
                if(clearPath(board, sx, sy, x, y)){
                    min_distance = std::min(min_distance, abs(x-sx));
                }
            }
            if(dx< 0 && dy < 0 && dir == CanonDirection::UP_LEFT){
                if(clearPath(board, sx, sy, x, y)){
                    min_distance = std::min(min_distance, abs(x-sx));
                }
            }
            if(dx > 0 && dy < 0 && dir == CanonDirection::DOWN_LEFT){
                if(clearPath(board, sx, sy, x, y)){
                    min_distance = std::min(min_distance, abs(x-sx));
                }
            }
            if(dir == CanonDirection::DOWN_RIGHT){
                if(dx > 0 && dy > 0 ){
                    if(clearPath(board, sx, sy, x, y)){
                        min_distance = std::min(min_distance, abs(x-sx));
                    }
                }
                else if(dx < 0 && dy < 0){
                    if(clearPath(board, sx, sy, x, y)){
                        min_distance = std::min(min_distance, width - abs(x-sx));
                    } 
                }
            }
        }
    }
    return min_distance==INT_MAX ? -1 : min_distance;
}

bool Player::clearPath(const Board& board, int sx, int sy, int tx, int ty) const {
    // As part of the prompt of shellIsComming, ChatGpt also suggested this function
    int dx = tx - sx;
    int dy = ty - sy;

    // Horizontal
    if (dx == 0 && dy != 0) {
        int step = (dy > 0) ? 1 : -1;
        for (int y = sy + step; y != ty; y += step) {
            if (board.isWallLocation(sx, y)) return false;
        }
        return true;
    }

    // Vertical
    if (dy == 0 && dx != 0) {
        int step = (dx > 0) ? 1 : -1;
        for (int x = sx + step; x != tx; x += step) {
            if (board.isWallLocation(x, sy)) return false;
        }
        return true;
    }

    // Digonal (45 degrees)
    if (abs(dx) == abs(dy)) {
        int stepX = (dx > 0) ? 1 : -1;
        int stepY = (dy > 0) ? 1 : -1;
        for (int i = 1; i < abs(dx); ++i) {
            int x = sx + i * stepX;
            int y = sy + i * stepY;
            if (board.isWallLocation(x, y)) return false;
        }
        return true;
    }
    return false;
}

bool Player::canShootFromLocation(const Board& board, const Tank& op_tank, const int x, const int y, const CanonDirection dir) const {
    int ox = op_tank.getLocationX();
    int oy = op_tank.getLocationY();
  
    if(dir == CanonDirection::UP && x > ox && y == oy){
        for(int i = ox + 1; i< x; i++){
            if(board.isWallLocation(i, oy)){
                return false;
            }
        }
        return true;
    }
    else if(dir == CanonDirection::DOWN && x < ox && y == oy){
        for(int i = x + 1; i< ox; i++){
            if(board.isWallLocation(i, oy)){
                return false;
            }
        }
        return true;
    }
    else if(dir == CanonDirection::LEFT && x == ox && y > oy){
        for(int i = oy + 1; i< y; i++){
            if(board.isWallLocation(ox, i)){
                return false;
            }
        }
        return true;
    }
    else if(dir == CanonDirection::RIGHT && x == ox && y < oy){
        for(int i = y + 1; i< oy; i++){
            if(board.isWallLocation(ox, i)){
                return false;
            }
        }
        return true;   
    }
    else if(dir == CanonDirection::UP_RIGHT && x > ox && y < oy && x - ox == oy - y){
        for(int i = ox + 1, j = oy - 1; i< x && j> y; i++, j--){
            if(board.isWallLocation(i, j)){
                return false;
            }
        }
        return true;
    }
    else if(dir == CanonDirection::UP_LEFT && x > ox && y > oy && x - ox == y - oy){
        for(int i = ox + 1, j = oy + 1; i< x && j< y; i++, j++){
            if(board.isWallLocation(i, j)){
                return false;
            }
        }
        return true;
    }
    else if(dir == CanonDirection::DOWN_LEFT && x < ox && y > oy && ox - x == y - oy){
        for(int i = x + 1, j = oy + 1; i< ox && j< y; i++, j++){
            if(board.isWallLocation(i, j)){
                return false;
            }
        }
        return true;
    }
    else if(dir == CanonDirection::DOWN_RIGHT && x < ox && y < oy && x - ox == y - oy){
        for(int i = x + 1, j = y + 1; i< ox && j < oy; i++, j++){
            if(board.isWallLocation(i, j)){
                return false;
            }
        }
        return true;
    }
    return false;
}

bool Player::canMove(const Board& board, int x, int y, const Tank& op_tank, const Tank& my_tank, int wanted_distance_from_shell) const{
    int shell_comming_turn_count = shellIsComming(board, my_tank, op_tank, x, y);
    return !board.isWallLocation(x, y) &&
     !board.isMineLocation(x, y) && 
     (op_tank.getLocationX() != x && op_tank.getLocationY() != y) &&
     (shell_comming_turn_count>wanted_distance_from_shell || shell_comming_turn_count == -1) ;
}