#ifndef BOARD_H
#define BOARD_H

#include "game_objects/GameObject.h"
#include "game_objects/Wall.h"
#include "game_objects/Mine.h"
#include "game_objects/Empty.h"
#include <vector>
#include <memory>

class GameManager; // Forward declaration to avoid circular dependency

class Board {
    

    public:
        Board(int rows, int columns);
        Board(){};
        // ~Board();
        // Board(const Board&) = delete;
        // Board& operator=(const Board&) = delete;

        Board(Board&&) = default;
        Board& operator=(Board&&) = default;

        int getRows() const;
        int getColumns() const;

        bool isWallLocation(int x, int y) const;
        bool isMineLocation(int x, int y) const;

        GameObject* getGameObjectAt(int x, int y) const;
        void setGameObjectAt(int x, int y, std::unique_ptr<GameObject> obj);     
        void readBoard(const std::string& path_input_file, GameManager& m );  // Implementation of reading the board from a file

    private:
        int rows;
        int columns;
        //std::vector<std::vector<GameObject*>> board;
        std::vector<std::vector<std::unique_ptr<GameObject>>> board;

        // Helper functions for readBoard
        void skipMetadata(std::ifstream& file);
        void processCell(char cell, int row, int col, GameManager& m, bool& has_errors, std::ostringstream& error_log);
        int readBoardLines(std::ifstream& file, int height, int width, GameManager& m, bool& has_errors, std::ostringstream& error_log);
        void fillMissingRows(int start_row, int height, int width, bool& has_errors, std::ostringstream& error_log);
        void writeErrorLog(const std::ostringstream& error_log);
};

#endif