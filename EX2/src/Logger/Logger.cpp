#include "Logger.h"
#include <iostream>
#include <iomanip>

Logger::Logger(const std::string& filename, LogLevel level)
    : level_(level) {
    log_file_.open(filename, std::ios::app);
    if (!log_file_.is_open()) {
        std::cerr << "Failed to open log file: " << filename << std::endl;
    }
}

Logger::~Logger() {
    if (log_file_.is_open()) {
        log_file_.close();
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    if (level < level_) return;

    if (log_file_.is_open()) {
        log_file_ << getTimestamp() << " [" << levelToString(level) << "] " << message << std::endl;
        log_file_.flush();
    }
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

void Logger::logDebug(const std::string& message) {
    log(LogLevel::DEBUG, message);
}

std::string Logger::getTimestamp() {
    std::time_t now = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buf);
}

std::string Logger::levelToString(LogLevel level) {
    switch(level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}