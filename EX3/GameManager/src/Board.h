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
    private:
        int rows;
        int columns;
        int wall_lives; // Number of lives a wall has before it is destroyed
        char wall_sign;
        char tank1_sign;
        char tank2_sign;
        char mine_sign;
        std::vector<std::vector<std::unique_ptr<GameObject>>> board;

        // Helper functions for readBoard
        void skipMetadata(std::ifstream& file); // Skips the metadata lines in the input file
        void processCell(char cell, int row, int col, GameManager& m, bool& has_errors, std::ostringstream& error_log); // Processes each cell in the board
        int readBoardLines(std::ifstream& file, int height, int width, GameManager& m, bool& has_errors, std::ostringstream& error_log); // Reads the board lines from the input file
        void fillMissingRows(int start_row, int height, int width, bool& has_errors, std::ostringstream& error_log); 
        void writeErrorLog(const std::ostringstream& error_log); // Writes the error log to a file

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
        void readBoard(const std::string& path_input_file, GameManager& m ); // Implementation of reading the board from a file    
};

#endif