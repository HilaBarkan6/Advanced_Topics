#include "Board.h"
#include "GameManager.h"

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

// void Board::skipMetadata(std::ifstream& file) {
//     std::string line;
//     constexpr int NUM_METADATA_LINES = 5;  // Description + MaxSteps + NumShells + Rows + Cols

//     for (int i = 0; i < NUM_METADATA_LINES; ++i) {
//         std::getline(file, line);
//     }
// }

// void Board::processCell(char cell, int row, int col, GameManager& m, bool& has_errors, std::ostringstream& error_log) {
//     if (cell == wall_sign) {
//         setGameObjectAt(row, col, std::make_unique<Wall>(wall_lives));
//     } 
//     else if (cell == mine_sign) {
//         setGameObjectAt(row, col, std::make_unique<Mine>());
//     } 
//     else if (cell ==tank1_sign) {
//         m.addTank(row, col, 1);
//         setGameObjectAt(row, col, std::make_unique<Empty>());
//     } 
//     else if (cell == tank2_sign) {
//         m.addTank(row, col, 2);
//         setGameObjectAt(row, col, std::make_unique<Empty>());
//     }
//      else if (cell == ' ') {
//         setGameObjectAt(row, col, std::make_unique<Empty>());
//     }
//     else {
//         setGameObjectAt(row, col, std::make_unique<Empty>());
//         has_errors = true;
//         error_log << "Unknown character '" << cell << "' at [" << row << ", " << col << "], treated as space.\n";
//     }
// }

// int Board::readBoardLines(std::ifstream& file, int height, int width, GameManager& m, bool& has_errors, std::ostringstream& error_log) {
//     std::string line;
//     int row = 0;
//     bool missing_col = false;
//     bool extra_col = false;

//     while (std::getline(file, line)) {
//         if (row >= height) {
//             has_errors = true;
//             error_log << "Too many rows, ignoring line " << row << ".\n";
//             continue;
//         }

//         int line_length = static_cast<int>(line.size());
//         if (line_length > width) {
//             extra_col = true;
//         }

//         for (int col = 0; col < width; ++col) {
//             char cell = (col < line_length) ? line[col] : ' ';
//             if (col >= line_length) {
//                 missing_col = true;
//             }
//             processCell(cell, row, col, m, has_errors, error_log);
//         }
//         ++row;
//     }

//     if (missing_col) {
//         has_errors = true;
//         error_log << "Some rows had missing columns, filled with spaces.\n";
//     }
//     if (extra_col) {
//         has_errors = true;
//         error_log << "Some rows had too many columns, ignored extra characters.\n";
//     }

//     return row;
// }

// void Board::fillMissingRows(int start_row, int height, int width, bool& has_errors, std::ostringstream& error_log) {
//     for (int row = start_row; row < height; ++row) {
//         for (int col = 0; col < width; ++col) {
//             setGameObjectAt(row, col, std::make_unique<Empty>());
//         }
//         has_errors = true;
//         error_log << "Row " << row << " is missing, filled with spaces.\n";
//     }
// }

// void Board::writeErrorLog(const std::ostringstream& error_log) {
//     std::ofstream err_file("input_errors.txt");
//     if (err_file.is_open()) {
//         err_file << error_log.str();
//     }
// }

// void Board::readBoard(const std::string& path_input_file, GameManager& m) {
//     std::ifstream file(path_input_file);
//     if (!file.is_open()) {
//         throw std::runtime_error("Error opening board file: " + path_input_file);
//     }

//     skipMetadata(file);

//     std::ostringstream error_log;
//     bool has_errors = false;

//     int height = rows;
//     int width = columns;

//     int rows_read = readBoardLines(file, height, width, m, has_errors, error_log);
//     fillMissingRows(rows_read, height, width, has_errors, error_log);

//     file.close();

//     if (has_errors) {
//         writeErrorLog(error_log);
//     }
// }

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