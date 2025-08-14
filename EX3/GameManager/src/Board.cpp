#include "Board.h"

using namespace GameManager_209399021_208239152;

Board::Board(int rows, int columns, int wall_lives, char wall_sign, char tank1_sign, char tank2_sign, char mine_sign) : rows(rows), columns(columns), wall_lives(wall_lives),
        wall_sign(wall_sign), tank1_sign(tank1_sign), tank2_sign(tank2_sign), mine_sign(mine_sign) {
    board.resize(columns);
    for (int i = 0; i < columns; ++i) {
        board[i].resize(rows);
    }
}

int Board::getRows() const {
    return rows;
}

int Board::getColumns() const {
    return columns;
}

bool Board::isWallLocation(int x, int y) const {
    if (x >= 0 && x < columns && y >= 0 && y < rows) {
        return dynamic_cast<Wall*>(board[x][y].get()) != nullptr;
    }
    return false;
}

bool Board::isMineLocation(int x, int y) const {
    if (x >= 0 && x < columns && y >= 0 && y < rows) {
        return dynamic_cast<Mine*>(board[x][y].get()) != nullptr;
    }
    return false;
}

void Board::setGameObjectAt(int x, int y, std::unique_ptr<GameObject> obj) {
    if (x >= 0 && x < columns && y >= 0 && y < rows) {
        board[x][y] = std::move(obj);
    }
}


void Board::reduceWallLives(int x, int y){
    Wall* wall = dynamic_cast<Wall*>(board[x][y].get());
    if (wall) {
        wall->reduceLife();
    }
}

bool Board::wallIsDestroyed(int x, int y) const {
    Wall* wall = dynamic_cast<Wall*>(board[x][y].get());
    return wall && wall->isDestroyed();
}