#ifndef BOARD_H
#define BOARD_H

#include "game_objects/GameObject.h"
#include "game_objects/Wall.h"
#include "game_objects/Mine.h"
#include "game_objects/Empty.h"
#include <vector>
#include <memory>

namespace GameManager_209399021_208239152 {
    class Board {
        private:
            int rows;
            int columns;
            int wall_lives; // Number of lives a wall has before it is destroyed
            char wall_sign;
            char tank1_sign;
            char tank2_sign;
            char mine_sign;
            std::vector<std::vector<std::unique_ptr<GameObject>>> board;

        public:
            Board(int rows, int columns, int wall_lives, char wall_sign, char tank1_sign, char tank2_sign, char mine_sign);
            Board(){};

            int getRows() const;
            int getColumns() const;

            bool isWallLocation(int x, int y) const;
            bool isMineLocation(int x, int y) const;
            void reduceWallLives(int x, int y); // Reduces the lives of a wall at the specified location
            bool wallIsDestroyed(int x, int y) const; // Checks if the wall at the specified location is destroyed

            void setGameObjectAt(int x, int y, std::unique_ptr<GameObject> obj); // Sets a game object at the specified location    
    };
}

#endif