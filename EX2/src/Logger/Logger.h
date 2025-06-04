#pragma once

#include <fstream>
#include <string>
#include <ctime>
#include <iostream>
#include <iomanip>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger {
private:
    std::ofstream log_file_;
    LogLevel level_;

    std::string getTimestamp();
    std::string levelToString(LogLevel level);
    
public:
    Logger(const std::string& filename, LogLevel level = LogLevel::INFO);
    ~Logger();

    void log(LogLevel level, const std::string& message);
    void logInfo(const std::string& message);
    void logError(const std::string& message);
    void logWarning(const std::string& message);
    void logDebug(const std::string& message);
};
