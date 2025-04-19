#include "OffensivePlayer.h"
#include <iostream>

Player::Action OffensivePlayer::getAction(const Board& board, const Tank& my_tank, const Tank& op_tank, const bool allow_shoot){

    int x = my_tank.getLocationX();
    int y = my_tank.getLocationY(); 

    if(shellIsComming(board, my_tank, op_tank, x, y) <=2 ){
        return defensiveMove(board, my_tank, op_tank);
    }
    return offensiveMove(board, my_tank, op_tank, allow_shoot);

}

Player::Action OffensivePlayer::defensiveMove(const Board& board, const Tank& my_tank, const Tank& op_tank){
    int x = my_tank.getLocationX();
    int y = my_tank.getLocationY();
    int dx = 0;
    int dy = 0;
    CanonDirection dir = my_tank.getCanonDirection();

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
    if(canMove(board, x+dx, y+dy, op_tank, my_tank)){
        return Player::Action::FORWARD;
    }
    else if(canMove(board, x-dx, y-dy, op_tank, my_tank)){
        return Player::Action::BACKWARD;
    }
    return Player::Action::ROTATE_025_LEFT;

}

Player::Action OffensivePlayer::offensiveMove(const Board& board, const Tank& my_tank, const Tank& op_tank, const bool allow_shoot){
    int x = my_tank.getLocationX();
    int y = my_tank.getLocationY();
    CanonDirection dir = my_tank.getCanonDirection();
    State start_state = {x, y, dir};
    q.push(QueueNode{start_state, Player::Action::NOTHING, 0});
    visited.insert(start_state);

    while(!q.empty()){
        QueueNode current = q.front();
        q.pop();

        int current_x = current.state.x;
        int current_y = current.state.y;
        CanonDirection current_dir = current.state.dir;

        // Should shoot
        if(canShootFromLocation(board, op_tank, current_x, current_y, current_dir)){
            return current.depth==0 && allow_shoot ? Player::Action::SHOOT : current.firstAction;
        }

        //Moving forward options
        int new_x = current_x;
        int new_y = current_y;
        switch(current_dir){
            case CanonDirection::UP: new_x--; break;
            case CanonDirection::DOWN: new_x++; break;  
            case CanonDirection::LEFT: new_y--; break;
            case CanonDirection::RIGHT: new_y++; break;
            case CanonDirection::UP_RIGHT: new_x--; new_y++; break;
            case CanonDirection::UP_LEFT: new_x--; new_y--; break;
            case CanonDirection::DOWN_LEFT: new_x++; new_y--; break;
            case CanonDirection::DOWN_RIGHT: new_x++; new_y++; break;
            default: break;
        }

        if(canMove(board, new_x, new_y, op_tank, my_tank)){
            State new_state = {new_x, new_y, current_dir};
            if(visited.find(new_state) == visited.end()){
                visited.insert(new_state);
                Player::Action act = current.depth == 0 ? Player::Action::FORWARD : current.firstAction;
                q.push(QueueNode{new_state, act, current.depth + 1});
            }
        }

        //Rotation options
        for(auto [rotationType, new_dir] : {
            std::make_pair(Player::Action::ROTATE_0125_LEFT, CanonDirection((int)current_dir - 1)),
            std::make_pair(Player::Action::ROTATE_025_LEFT, CanonDirection((int)current_dir - 2)),
            std::make_pair(Player::Action::ROTATE_0125_RIGHT, CanonDirection((int)current_dir + 1)),
            std::make_pair(Player::Action::ROTATE_025_RIGHT, CanonDirection((int)current_dir + 2))
        }){
            State new_state = {current_x, current_y, new_dir};
            if(visited.find(new_state) == visited.end()){
                visited.insert(new_state);
                Player::Action act = current.depth == 0 ? rotationType : current.firstAction;
                q.push(QueueNode{new_state, act, current.depth + 1});
            }
        }
        
    }
    Player::Action::BACKWARD;
}