#ifndef BOARD_H
#define BOARD_H

#include "game_objects/GameObject.h"
#include "game_objects/Wall.h"
#include "game_objects/Mine.h"
#include "game_objects/Empty.h"
#include <vector>
#include <memory>

class Board{
    

    public:
        Board(int rows, int columns);
        Board(){};
        // ~Board();

        int getRows() const;
        int getColumns() const;

        bool isWallLocation(int x, int y) const;
        bool isMineLocation(int x, int y) const;

        std::unique_ptr<GameObject> getGameObjectAt(int x, int y) const;
        void setGameObjectAt(int x, int y, std::unique_ptr<GameObject> obj);     

    private:
        int rows;
        int columns;
        //std::vector<std::vector<GameObject*>> board;
        std::vector<std::vector<std::unique_ptr<GameObject>>> board;
};

#endif