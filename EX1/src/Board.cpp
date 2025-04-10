#include "Board.h"

Board::Board(int rows, int columns) : rows(rows), columns(columns), board(rows, std::vector<GameObject*>(columns, nullptr)) {   
}

Board::~Board() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            delete board[i][j]; // Clean up dynamically allocated GameObjects
        }
    }
}

void Board::setGameObjectAt(int x, int y, GameObject* obj) {
    if (x >= 0 && x < rows && y >= 0 && y < columns) {
        board[x][y] = obj;
    }
}

bool Board::isWallLocation(int x, int y) const {
    if (x >= 0 && x < rows && y >= 0 && y < columns) {
        return dynamic_cast<Wall*>(board[x][y]) != nullptr;
    }
    return false;
}

bool Board::isMineLocation(int x, int y) const {
    if (x >= 0 && x < rows && y >= 0 && y < columns) {
        return dynamic_cast<Mine*>(board[x][y]) != nullptr;
    }
    return false;
}

bool Board::isEmptyLocation(int x, int y) const{
    if (x >= 0 && x < rows && y >= 0 && y < columns) {
        return dynamic_cast<Empty*>(board[x][y]) != nullptr;
    }
    return false;
}

GameObject* Board::getGameObjectAt(int x, int y) const {
    if (x >= 0 && x < rows && y >= 0 && y < columns) {
        return board[x][y];
    }
    return nullptr;
}