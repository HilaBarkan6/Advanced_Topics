#ifndef BoardReader_H
#define BoardReader_H

#include <string>
#include <fstream>
#include <vector>
#include <filesystem>

#include "../../UserCommon/Logger/Logger.h"
#include "../../UserCommon/configuration/Config.h"

using namespace UserCommon_209399021_208239152;

struct GameInput{
    std::vector<std::vector<char>> board;
    std::string input_file_name;
    int max_steps;
    int num_shells;
    int height;
    int width;
};

static constexpr const char WALL_SIGN = '#';
static constexpr const char TANK1_SIGN = '1';
static constexpr const char TANK2_SIGN = '2';
static constexpr const char MINE_SIGN = '@';

//Logger logger;

GameInput readBoard(const std::string& path_input_file);
void readGameParameters(std::ifstream& file, GameInput& input);
int readIntValueFromLine(const std::string& line, const std::string& key);

// read the board matrix itself
void readBoardData(const std::string& path_input_file, GameInput& input);
void skipMetadata(std::ifstream& file);
int readBoardLines(std::ifstream& file, GameInput& input, bool& has_errors, std::ostringstream& error_log);
void processCell(char cell, GameInput& input, int row, int col, bool& has_errors, std::ostringstream& error_log);
void fillMissingRows(int start_row, GameInput& input, bool& has_errors, std::ostringstream& error_log);
void writeErrorLog(const std::ostringstream& error_log);



#endif // BoardReader_H