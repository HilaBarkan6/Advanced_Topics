#include "Player.h"
#include <cmath>

int Player::shellIsComming(const Board& board, const Tank& my_tank, const Tank& op_tank, int x_to_check, int y_to_check) const{
    int x = x_to_check;
    int y = y_to_check;
    int width = board.getColumns();
    int height = board.getRows();
    
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
                    if(clearPathFromSrcToDst(board, sx, sy, x, y, dir)){
                        min_distance = std::min(min_distance, abs(y-sy));
                    }
                }
                else{
                    if(clearPathFromSrcToDst(board, sx, sy, x, y, dir)){
                        min_distance = std::min(min_distance, width - abs(y-sy));
                    } 
                }
            }
            
            if(dir == CanonDirection::LEFT){
                if (sy > y){
                    if(clearPathFromSrcToDst(board, sx, sy, x, y, dir)){
                        min_distance = std::min(min_distance, abs(y-sy));
                    }
                }
                else{
                    if(clearPathFromSrcToDst(board, sx, sy, x, y, dir)){
                        min_distance = std::min(min_distance, width - abs(y-sy));
                    }
                }
            }
        }
        if(sy == y){
            if(dir == CanonDirection::DOWN){
                if(sx < x){
                    if(clearPathFromSrcToDst(board, sx, sy, x, y, dir)){
                        min_distance = std::min(min_distance, abs(x-sx));
                    }
                }
                else{
                    if(clearPathFromSrcToDst(board, sx, sy, x, y, dir)){
                        min_distance = std::min(min_distance, height - abs(x-sx));
                    }
                }
            }
            if(dir == CanonDirection::UP){
                if (sx > x){
                    if(clearPathFromSrcToDst(board, sx, sy, x, y, dir)){
                        min_distance = std::min(min_distance, abs(x-sx));
                    }
                }
                else{
                    if(clearPathFromSrcToDst(board, sx, sy, x, y, dir)){
                        min_distance = std::min(min_distance, height - abs(x-sx));
                    }
                }
            }
        }
        int dx = (x - sx + height) % height;
        int dy = (y - sy + width) % width;
        if(abs(dx) == abs(dy)){
            if(dx < 0 && dy > 0 && dir == CanonDirection::UP_RIGHT){
                if(clearPathFromSrcToDst(board, sx, sy, x, y, dir)){
                    min_distance = std::min(min_distance, abs(x-sx));
                }
            }
            if(dx< 0 && dy < 0 && dir == CanonDirection::UP_LEFT){
                if(clearPathFromSrcToDst(board, sx, sy, x, y, dir)){
                    min_distance = std::min(min_distance, abs(x-sx));
                }
            }
            if(dx > 0 && dy < 0 && dir == CanonDirection::DOWN_LEFT){
                if(clearPathFromSrcToDst(board, sx, sy, x, y, dir)){
                    min_distance = std::min(min_distance, abs(x-sx));
                }
            }
            if(dx > 0 && dy > 0 && dir == CanonDirection::DOWN_RIGHT){  
                if(clearPathFromSrcToDst(board, sx, sy, x, y, dir)){
                    min_distance = std::min(min_distance, abs(x-sx));
                }
                  
            }
        }
    }
    return min_distance==INT_MAX ? -1 : min_distance;
}

bool Player::canShootFromLocation(const Board& board, const Tank& op_tank, const int x, const int y, const CanonDirection dir) const{
   return clearPathFromSrcToDst(board, x, y, op_tank.getLocationX(), op_tank.getLocationY(), dir);
}

bool Player::canMove(const Board& board, int x, int y, const Tank& op_tank, const Tank& my_tank, int wanted_distance_from_shell) const{
    int shell_comming_turn_count = shellIsComming(board, my_tank, op_tank, x, y);
    return !board.isWallLocation(x, y) &&
     !board.isMineLocation(x, y) && 
     (op_tank.getLocationX() != x || op_tank.getLocationY() != y) &&
     (shell_comming_turn_count>wanted_distance_from_shell || shell_comming_turn_count == -1) ;
}

bool Player::clearPathFromSrcToDst(const Board& board, const int src_x, const int src_y, const int dst_x, const int dst_y, const CanonDirection dir) const{
    int cx = src_x;
    int cy = src_y;
    int ox = dst_x;
    int oy = dst_y;
    int cols = board.getColumns();
    int rows = board.getRows();
    for (int steps = 0; steps < std::max(rows, cols); ++steps) {
        if (cx == ox && cy == oy) {
            return true;
        }

        if (board.isWallLocation(cx, cy)) {
            return false;
        }

        switch (dir) {
            case CanonDirection::UP:
                cx = (cx - 1 + rows) % rows;
                break;
            case CanonDirection::DOWN:
                cx = (cx + 1) % rows;
                break;
            case CanonDirection::LEFT:
                cy = (cy - 1 + cols) % cols;
                break;
            case CanonDirection::RIGHT:
                cy = (cy + 1) % cols;
                break;
            case CanonDirection::UP_RIGHT:
                cx = (cx - 1 + rows) % rows;
                cy = (cy + 1) % cols;
                break;
            case CanonDirection::UP_LEFT:
                cx = (cx - 1 + rows) % rows;
                cy = (cy - 1 + cols) % cols;
                break;
            case CanonDirection::DOWN_RIGHT:
                cx = (cx + 1) % rows;
                cy = (cy + 1) % cols;
                break;
            case CanonDirection::DOWN_LEFT:
                cx = (cx + 1) % rows;
                cy = (cy - 1 + cols) % cols;
                break;
        }
    }
    return false; 
}