#include "NetworkLayer.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

NetworkLayer::NetworkLayer(Logger* logger) : logger(logger) {}

void NetworkLayer::tcpScan(const std::string& target, int startPort, int endPort) {
    logger->log(INFO, "Starting TCP Scan...");

    for (int port = startPort; port <= endPort; ++port) {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            logger->log(ERROR, "Failed to create TCP socket.");
            continue;
        }

        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET, target.c_str(), &addr.sin_addr);

        if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) >= 0) {
            logger->log(INFO, "Port " + std::to_string(port) + " is open (TCP)");
        }

        close(sock);
    }
}

void NetworkLayer::udpScan(const std::string& target, int startPort, int endPort) {
    logger->log(INFO, "Starting UDP Scan...");

    for (int port = startPort; port <= endPort; ++port) {
        int sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock < 0) {
            logger->log(ERROR, "Failed to create UDP socket.");
            continue;
        }

        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET, target.c_str(), &addr.sin_addr);

        char buf[1] = {0};
        sendto(sock, buf, sizeof(buf), 0, (struct sockaddr*)&addr, sizeof(addr));

        close(sock);
        logger->log(INFO, "UDP packet sent to port " + std::to_string(port));
    }
}
