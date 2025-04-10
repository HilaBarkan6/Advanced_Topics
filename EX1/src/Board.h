#ifndef BOARD_H
#define BOARD_H

#include "game_objects/GameObject.h"
#include "game_objects/Wall.h"
#include "game_objects/Mine.h"
#include "game_objects/Empty.h"
#include <vector>


class Board{
    private:
        int rows;
        int columns;
        std::vector<std::vector<GameObject*>> board; // 2D vector of GameObject pointers

    public:
        Board(int rows, int columns);
        Board(){};
        ~Board(); // Destructor to clean up dynamically allocated memory

        bool isWallLocation(int x, int y) const;
        bool isMineLocation(int x, int y) const;
        bool isEmptyLocation(int x, int y) const;
        void setGameObjectAt(int x, int y, GameObject* obj);
        GameObject* getGameObjectAt(int x, int y) const;
};


#endif