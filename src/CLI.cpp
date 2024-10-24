#include "CLI.h"
#include <iostream>

CLI::CLI(Logger* logger, NetworkLayer* networkLayer, TaskScheduler* taskScheduler, SessionManager* sessionManager)
    : logger(logger), networkLayer(networkLayer), taskScheduler(taskScheduler), sessionManager(sessionManager) {}

std::map<std::string, std::string> CLI::parseArguments(int argc, char* argv[]) {
    std::map<std::string, std::string> options;

    for (int i = 1; i < argc; i += 2) {
        if (i + 1 < argc) {
            options[argv[i]] = argv[i + 1];
        }
    }

    return options;
}

void CLI::run(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./your_program <command> [options]\n";
        std::cerr << "Commands:\n";
        std::cerr << "  tcp_scan  -target <IP> -start <startPort> -end <endPort>\n";
        std::cerr << "  udp_scan  -target <IP> -start <startPort> -end <endPort>\n";
        std::cerr << "  session   -start <IP> -escalate\n";
        return;
    }

    std::string command = argv[1];
    auto options = parseArguments(argc - 1, argv + 1);

    if (command == "tcp_scan") {
        if (options.count("-target") && options.count("-start") && options.count("-end")) {
            std::string target = options["-target"];
            int startPort = std::stoi(options["-start"]);
            int endPort = std::stoi(options["-end"]);

            taskScheduler->addTask([=]() {
                networkLayer->tcpScan(target, startPort, endPort);
            });
        } else {
            logger->log(ERROR, "Missing options for TCP scan.");
        }
    } else if (command == "udp_scan") {
        if (options.count("-target") && options.count("-start") && options.count("-end")) {
            std::string target = options["-target"];
            int startPort = std::stoi(options["-start"]);
            int endPort = std::stoi(options["-end"]);

            taskScheduler->addTask([=]() {
                networkLayer->udpScan(target, startPort, endPort);
            });
        } else {
            logger->log(ERROR, "Missing options for UDP scan.");
        }
    } else if (command == "session") {
        if (options.count("-start")) {
            std::string target = options["-start"];
            sessionManager->startSession(target);

            if (options.count("-escalate")) {
                sessionManager->escalatePrivileges("session_id");
            }
        } else {
            logger->log(ERROR, "Missing options for session management.");
        }
    } else {
        logger->log(ERROR, "Unknown command: " + command);
        std::cerr << "Unknown command: " << command << "\n";
    }
}
