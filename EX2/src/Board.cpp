#include "Board.h"

Board::Board(int rows, int columns) : rows(rows), columns(columns){}

// Board::~Board() {
//     for (int i = 0; i < rows; ++i) {
//         for (int j = 0; j < columns; ++j) {
//             delete board[i][j]; // Clean up dynamically allocated GameObjects
//         }
//     }
// }

int Board::getRows() const {
    return rows;
}

int Board::getColumns() const {
    return columns;
}

bool Board::isWallLocation(int x, int y) const {
    if (x >= 0 && x < rows && y >= 0 && y < columns) {
        return dynamic_cast<Wall*>(board[x][y].get()) != nullptr;
    }
    return false;
}

bool Board::isMineLocation(int x, int y) const {
    if (x >= 0 && x < rows && y >= 0 && y < columns) {
        return dynamic_cast<Mine*>(board[x][y].get()) != nullptr;
    }
    return false;
}

GameObject* Board::getGameObjectAt(int x, int y) const {
    if (x >= 0 && x < rows && y >= 0 && y < columns) {
        return board[x][y].get();
    }
    return nullptr;
}

void Board::setGameObjectAt(int x, int y, std::unique_ptr<GameObject> obj) {
    if (x >= 0 && x < rows && y >= 0 && y < columns) {
        board[x][y] = std::move(obj);
    }
}

