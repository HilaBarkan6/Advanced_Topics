#include "Player.h"
#include <cmath>

Player::Player(int id) : id(id) {}


int Player::getId() const {
    return id;
}

//TODO - this return
int Player::shellIsComming(const Board& board, const Tank& my_tank, const Tank& op_tank, int x_to_check, int y_to_check) const{
    int x = x_to_check;
    int y = y_to_check;
    
    const std::vector<Shell *>& flying_shells1 = op_tank.getFlyingShells();
    const std::vector<Shell *>& flying_shells2 = my_tank.getFlyingShells();
    std::vector<Shell *> all_shells;
    all_shells.insert(all_shells.end(), flying_shells1.begin(), flying_shells1.end());
    all_shells.insert(all_shells.end(), flying_shells2.begin(), flying_shells2.end());

    int min_distance = INT_MAX;
    for(Shell * shell : all_shells){
        std::pair<int, int> shell_location = shell->getLocation();
        int sx = shell_location.first;
        int sy = shell_location.second;
        CanonDirection dir = shell->getFlyingDirection();
        if(sx == x){
            if(sy < y && dir == CanonDirection::RIGHT){
                if(clearPath(board, sx, sy, x, y)){
                    min_distance = std::min(min_distance, abs(y-sy));
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
            if(dx > 0 && dy > 0 && dir == CanonDirection::DOWN_RIGHT){
                if(clearPath(board, sx, sy, x, y)){
                    min_distance = std::min(min_distance, abs(x-sx));
                }
            }
        }
    }
    return min_distance==INT_MAX ? -1 : min_distance;
}

bool Player::clearPath(const Board& board, int sx, int sy, int tx, int ty) const {
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
    std::pair<int, int> my_location = std::make_pair(x, y);
    std::pair<int, int> op_location = op_tank.getLocation();

    
    if(dir == CanonDirection::UP && my_location.first > op_location.first && my_location.second == op_location.second){
        for(int i = op_location.first + 1; i< my_location.first; i++){
            if(board.isWallLocation(i, op_location.second)){
                return false;
            }
        }
        return true;
    }
    else if(dir == CanonDirection::DOWN && my_location.first < op_location.first && my_location.second == op_location.second){
        for(int i = my_location.first + 1; i< op_location.first; i++){
            if(board.isWallLocation(i, op_location.second)){
                return false;
            }
        }
        return true;
    }
    else if(dir == CanonDirection::LEFT && my_location.first == op_location.first && my_location.second > op_location.second){
        for(int i = op_location.second + 1; i< my_location.second; i++){
            if(board.isWallLocation(op_location.first, i)){
                return false;
            }
        }
        return true;
    }
    else if(dir == CanonDirection::RIGHT && my_location.first == op_location.first && my_location.second < op_location.second){
        for(int i = my_location.second + 1; i< op_location.second; i++){
            if(board.isWallLocation(op_location.first, i)){
                return false;
            }
        }
        return true;
        
    }
   
    else if(dir == CanonDirection::UP_RIGHT && my_location.first > op_location.first && my_location.second < op_location.second && my_location.first - op_location.first == op_location.second - my_location.second){
        for(int i = op_location.first + 1, j = op_location.second - 1; i< my_location.first && j> my_location.second; i++, j--){
            if(board.isWallLocation(i, j)){
                return false;
            }
        }
        return true;
    }
    else if(dir == CanonDirection::UP_LEFT && my_location.first > op_location.first && my_location.second > op_location.second && my_location.first - op_location.first == my_location.second - op_location.second){
        for(int i = op_location.first + 1, j = op_location.second + 1; i< my_location.first && j< my_location.second; i++, j++){
            if(board.isWallLocation(i, j)){
                return false;
            }
        }
        return true;
    }
    else if(dir == CanonDirection::DOWN_LEFT && my_location.first < op_location.first && my_location.second > op_location.second && op_location.first - my_location.first == my_location.second - op_location.second){
        for(int i = my_location.first + 1, j = op_location.second + 1; i< op_location.first && j< my_location.second; i++, j++){
            if(board.isWallLocation(i, j)){
                return false;
            }
        }
        return true;
    }
    else if(dir == CanonDirection::DOWN_RIGHT && my_location.first < op_location.first && my_location.second < op_location.second && my_location.first - op_location.first == my_location.second - op_location.second){
        for(int i = my_location.first + 1, j = my_location.second + 1; i< op_location.first && j < op_location.second; i++, j++){
            if(board.isWallLocation(i, j)){
                return false;
            }
        }
        return true;
    }
    return false;
}


bool Player::canMove(const Board& board, int x, int y, const Tank& op_tank, const Tank& my_tank) const{
    return !board.isWallLocation(x, y) &&
     !board.isMineLocation(x, y) && 
     (op_tank.getLocationX() != x && op_tank.getLocationY() != y) &&
     shellIsComming(board, my_tank, op_tank, x, y)>2;
}