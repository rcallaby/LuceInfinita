#include "Logger.h"
#include <iostream>

Logger::Logger(const std::string& filename) {
    logFile.open(filename, std::ios::out | std::ios::app);
    if (!logFile) {
        std::cerr << "Failed to open log file: " << filename << std::endl;
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(mtx);
    std::string levelStr;
    switch (level) {
        case INFO:    levelStr = "[INFO]"; break;
        case WARNING: levelStr = "[WARNING]"; break;
        case ERROR:   levelStr = "[ERROR]"; break;
    }

    if (logFile.is_open()) {
        logFile << levelStr << " " << message << std::endl;
    }

    std::cout << levelStr << " " << message << std::endl;
}
