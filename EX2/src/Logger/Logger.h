#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <ctime>
#include <iostream>
#include <iomanip>

enum class LogLevel {
    INFO,
    WARNING,
    ERROR
};

class Logger {
private:
    std::ofstream log_file_;

    std::string getTimestamp();
    std::string levelToString(LogLevel level);
    
public:
    Logger(){};
    ~Logger();

    void log(LogLevel level, const std::string& message);
    void logInfo(const std::string& message);
    void logError(const std::string& message);
    void logWarning(const std::string& message);
    void setLogFile(const std::string& filename);
};

#endif