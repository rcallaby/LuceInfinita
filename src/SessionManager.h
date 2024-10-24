#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <string>
#include <map>
#include <mutex>
#include "Logger.h"

class SessionManager {
private:
    Logger* logger;
    std::map<std::string, std::string> activeSessions; // Maps session ID to target IP
    std::mutex mtx; // Ensures thread safety for session management

    // Helper method to generate unique session IDs
    std::string generateSessionID();

public:
    SessionManager(Logger* logger);

    // Starts a session with a given target
    std::string startSession(const std::string& target);

    // Attempts privilege escalation for a given session ID
    bool escalatePrivileges(const std::string& sessionID);

    // Ends a session by session ID
    void endSession(const std::string& sessionID);

    // Lists all active sessions
    void listSessions();
};

#endif // SESSION_MANAGER_H
