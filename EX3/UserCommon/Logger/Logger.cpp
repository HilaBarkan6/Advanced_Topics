#include "Logger.h"

using namespace UserCommon_209399021_208239152;

Logger::~Logger() {
    if (log_file_.is_open()) {
        log_file_.close();
    }
}

void Logger::log(LogLevel level, const std::string& message) {

    std::string levelStr = levelToString(level);
    
    std::string colorStart = "";
    std::string colorEnd = "\033[0m";

    if (level == LogLevel::INFO) colorStart = "\033[32m";     // greeen for INFO
    else if (level == LogLevel::WARNING) colorStart = "\033[33m"; // yellow for WARNING
    else if (level == LogLevel::ERROR) colorStart = "\033[31m";   // red for ERROR
    else colorStart = "\033[0m"; 

    std::string timestamp = getTimestamp();
    std::string logMessage = timestamp + " [" + levelStr + "] " + message;

    if (log_file_.is_open()) {
        log_file_ << logMessage << std::endl; 
        log_file_.flush();
    }

    
    std::cout << timestamp << colorStart << " [" + levelStr + "] " << colorEnd <<  message  << std::endl;
}


void Logger::logInfo(const std::string& message) {
    log(LogLevel::INFO, message);
}

void Logger::logError(const std::string& message) {
    log(LogLevel::ERROR, message);
}

void Logger::logWarning(const std::string& message) {
    log(LogLevel::WARNING, message);
}

std::string Logger::getTimestamp() {
    std::time_t now = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buf);
}

std::string Logger::levelToString(LogLevel level) {
    switch(level) {
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

void Logger::setLogFile(const std::string& filename) {
    if (log_file_.is_open()) {
        log_file_.close();
    }

    log_file_.open(filename, std::ios::out);
    if (!log_file_.is_open()) {
        std::cerr << "Failed to open log file: " << filename << std::endl;
    }
}