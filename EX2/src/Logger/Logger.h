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
    virtual ~Logger();
    // delete copy constructor and assignment operator
    Logger(const Logger&) = delete; 
    Logger& operator=(const Logger&) = delete;
    // delete move constructor and assignment operator
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

    void log(LogLevel level, const std::string& message);
    void logInfo(const std::string& message);
    void logError(const std::string& message);
    void logWarning(const std::string& message);
    void setLogFile(const std::string& filename);
};

#endif