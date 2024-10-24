#include "SessionManager.h"
#include <iostream>
#include <sstream>
#include <random>

SessionManager::SessionManager(Logger* logger) : logger(logger) {}

std::string SessionManager::generateSessionID() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1000, 9999);

    std::stringstream ss;
    ss << "session_" << dis(gen);
    return ss.str();
}

std::string SessionManager::startSession(const std::string& target) {
    std::lock_guard<std::mutex> lock(mtx);
    std::string sessionID = generateSessionID();
    activeSessions[sessionID] = target;

    logger->log(INFO, "Started session with target: " + target + " (Session ID: " + sessionID + ")");
    return sessionID;
}

bool SessionManager::escalatePrivileges(const std::string& sessionID) {
    std::lock_guard<std::mutex> lock(mtx);
    if (activeSessions.find(sessionID) != activeSessions.end()) {
        std::string target = activeSessions[sessionID];

        // Simulate privilege escalation
        logger->log(INFO, "Attempting privilege escalation on session: " + sessionID + " (Target: " + target + ")");
        // In a real implementation, this would involve actual privilege escalation steps
        bool success = true; // Simulate success

        if (success) {
            logger->log(INFO, "Privilege escalation successful on session: " + sessionID);
            return true;
        } else {
            logger->log(ERROR, "Privilege escalation failed on session: " + sessionID);
            return false;
        }
    } else {
        logger->log(ERROR, "Session ID not found: " + sessionID);
        return false;
    }
}

void SessionManager::endSession(const std::string& sessionID) {
    std::lock_guard<std::mutex> lock(mtx);
    if (activeSessions.erase(sessionID) > 0) {
        logger->log(INFO, "Ended session: " + sessionID);
    } else {
        logger->log(ERROR, "Failed to end session. Session ID not found: " + sessionID);
    }
}

void SessionManager::listSessions() {
    std::lock_guard<std::mutex> lock(mtx);
    logger->log(INFO, "Listing active sessions:");

    if (activeSessions.empty()) {
        std::cout << "No active sessions.\n";
    } else {
        for (const auto& session : activeSessions) {
            std::cout << "Session ID: " << session.first << " - Target: " << session.second << "\n";
        }
    }
}
