#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <mutex>
#include <fstream>

enum LogLevel {
    INFO,
    WARNING,
    ERROR
};

class Logger {
private:
    std::ofstream logFile;
    std::mutex mtx;

public:
    Logger(const std::string& filename);
    ~Logger();

    void log(LogLevel level, const std::string& message);
};

#endif // LOGGER_H
