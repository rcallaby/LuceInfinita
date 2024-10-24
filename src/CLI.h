#ifndef CLI_H
#define CLI_H

#include <string>
#include <vector>
#include <map>
#include "Logger.h"
#include "NetworkLayer.h"
#include "TaskScheduler.h"
#include "SessionManager.h"

class CLI {
private:
    Logger* logger;
    NetworkLayer* networkLayer;
    TaskScheduler* taskScheduler;
    SessionManager* sessionManager;

    // Helper method to parse command-line arguments
    std::map<std::string, std::string> parseArguments(int argc, char* argv[]);

public:
    CLI(Logger* logger, NetworkLayer* networkLayer, TaskScheduler* taskScheduler, SessionManager* sessionManager);

    // Main method to handle user input and initiate scans
    void run(int argc, char* argv[]);
};

#endif // CLI_H
