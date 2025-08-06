#include "BoardReader.h"
using namespace UserCommon_209399021_208239152;

GameInput readBoard(const std::string& path_input_file){
    // size_t last_slash = path_input_file.find_last_of("/\\");
    // const std::string input_file_name = (last_slash == std::string::npos) ? path_input_file : path_input_file.substr(last_slash + 1);
    const std::string input_file_name = std::filesystem::path(path_input_file).stem().string();

    std::ifstream file(path_input_file);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + path_input_file);
    }

    GameInput game_input;
    game_input.input_file_name = input_file_name; // Store the input file name
    readGameParameters(file, game_input); 

    game_input.board.resize(game_input.width); // total rows
    for (auto& row : game_input.board)
        row.resize(game_input.height);  

    readBoardData(path_input_file, game_input);

    //logger.logInfo("Board successfully built");
    file.close();
    return game_input;
}

void readGameParameters(std::ifstream& file, GameInput& input) {
    std::string line;

    // Line 1 - Description (ignored)
    std::getline(file, line);

    if (!std::getline(file, line)) {
        //logger.logError("Missing MaxSteps line.");
        throw std::runtime_error("Missing MaxSteps line.");
    }

    input.max_steps = readIntValueFromLine(line, "MaxSteps");

    if (!std::getline(file, line)) {
        //logger.logError("Missing NumShells line.");
        throw std::runtime_error("Missing NumShells line.");
    }
    input.num_shells = readIntValueFromLine(line, "NumShells");

    if (!std::getline(file, line)) {
        //logger.logError("Missing Rows line.");
        throw std::runtime_error("Missing Rows line.");
    }
    input.height = readIntValueFromLine(line, "Rows");

    if (!std::getline(file, line)) {
        //logger.logError("Missing Cols line.");
        throw std::runtime_error("Missing Cols line.");
    }
    input.width = readIntValueFromLine(line, "Cols");
}

int readIntValueFromLine(const std::string& line, const std::string& key) {
    size_t pos = line.find("=");
    if (pos == std::string::npos) {
        //logger.logError("Invalid " + key + " line: " + line);
        throw std::runtime_error("Invalid " + key + " line.");
    }
    try {
        return std::stoi(line.substr(pos + 1));
    } catch (...) {
        //logger.logError("Invalid " + key + " value.");
        throw std::runtime_error("Invalid " + key + " value.");
    }
}

void skipMetadata(std::ifstream& file) {
    std::string line;
    constexpr int NUM_METADATA_LINES = 5;  // Description + MaxSteps + NumShells + Rows + Cols

    for (int i = 0; i < NUM_METADATA_LINES; ++i) {
        std::getline(file, line);
    }
}

void processCell(char cell, GameInput& input, int row, int col, bool& has_errors, std::ostringstream& error_log) {
    const Config& config = Config::getInstance();
    //TODO - can be simplified

    if (cell == config.get("wall_sign", std::string(1, WALL_SIGN))[0]) {
        input.board[col][row] = cell;
    } 
    else if (cell == config.get("mine_sign", std::string(1, MINE_SIGN))[0]) {
        input.board[col][row] = cell;
    } 
    else if (cell == config.get("tank1_sign", std::string(1, TANK1_SIGN))[0]) {
        input.board[col][row] = cell;
    } 
    else if (cell == config.get("tank2_sign", std::string(1, TANK2_SIGN))[0]) {
        input.board[col][row] = cell;
    }
     else if (cell == ' ') {
        input.board[col][row] = ' ';
    }
    else {
        input.board[col][row] = ' ';
        has_errors = true;
        error_log << "Unknown character '" << cell << "' at [" << row << ", " << col << "], treated as space.\n";
    }
}

int readBoardLines(std::ifstream& file, GameInput& input, bool& has_errors, std::ostringstream& error_log) {
    std::string line;
    int row = 0;
    bool missing_col = false;
    bool extra_col = false;

    while (std::getline(file, line)) {
        if (row >= input.height) {
            has_errors = true;
            error_log << "Too many rows, ignoring line " << row << ".\n";
            continue;
        }

        int line_length = static_cast<int>(line.size());
        if (line_length > input.width) {
            extra_col = true;
        }

        for (int col = 0; col < input.width; ++col) {
            char cell = (col < line_length) ? line[col] : ' ';
            if (col >= line_length) {
                missing_col = true;
            }
            processCell(cell, input, row, col, has_errors, error_log);
        }
        ++row;
    }

    if (missing_col) {
        has_errors = true;
        error_log << "Some rows had missing columns, filled with spaces.\n";
    }
    if (extra_col) {
        has_errors = true;
        error_log << "Some rows had too many columns, ignored extra characters.\n";
    }

    return row;
}

void fillMissingRows(int start_row, GameInput& input, bool& has_errors, std::ostringstream& error_log) {
    for (int row = start_row; row < input.height; ++row) {
        for (int col = 0; col < input.width; ++col) {
            input.board[row][col] = ' ';
        }
        has_errors = true;
        error_log << "Row " << row << " is missing, filled with spaces.\n";
    }
}

void writeErrorLog(const std::ostringstream& error_log) {
    std::ofstream err_file("input_errors.txt");
    if (err_file.is_open()) {
        err_file << error_log.str();
    }
}

void readBoardData(const std::string& path_input_file, GameInput& input) {
    std::ifstream file(path_input_file);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening board file: " + path_input_file);
    }

    skipMetadata(file);

    std::ostringstream error_log;
    bool has_errors = false;

    int rows_read = readBoardLines(file, input, has_errors, error_log);
    fillMissingRows(rows_read, input, has_errors, error_log);

    if (has_errors) {
        writeErrorLog(error_log);
    }
}