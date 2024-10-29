#include "ServiceEnumeration.hpp"
#include <iostream>
#include <fstream>
#include <regex>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

ServiceEnumeration::ServiceEnumeration(const std::string& databaseFile) {
    loadDatabase(databaseFile);
}

void ServiceEnumeration::loadDatabase(const std::string& databaseFile) {
    std::ifstream file(databaseFile);
    if (!file.is_open()) {
        std::cerr << "Error loading service database from file: " << databaseFile << std::endl;
        return;
    }

    std::string line;
    while (getline(file, line)) {
        std::istringstream iss(line);
        std::string serviceName, version, signature;
        if (!(iss >> serviceName >> version >> signature)) continue;

        serviceDatabase[serviceName].push_back({serviceName, version, signature});
    }

    file.close();
}

std::vector<ServiceFingerprint> ServiceEnumeration::enumerateService(const std::string& ip, int port) {
    std::string banner = grabBanner(ip, port);
    std::vector<ServiceFingerprint> detectedServices;

    if (!banner.empty()) {
        ServiceFingerprint matchedService = matchService(banner);
        if (!matchedService.name.empty()) {
            detectedServices.push_back(matchedService);
        }
    }
    return detectedServices;
}

std::string ServiceEnumeration::grabBanner(const std::string& ip, int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Socket creation failed." << std::endl;
        return "";
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

    if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed to " << ip << ":" << port << std::endl;
        close(sock);
        return "";
    }

    // Example for HTTP service
    std::string request = "HEAD / HTTP/1.0\r\n\r\n";
    send(sock, request.c_str(), request.length(), 0);

    char buffer[4096];
    int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);
    if (bytesReceived < 0) {
        std::cerr << "Error receiving data." << std::endl;
        close(sock);
        return "";
    }

    std::string banner(buffer, bytesReceived);
    close(sock);
    return banner;
}

ServiceFingerprint ServiceEnumeration::matchService(const std::string& banner) {
    for (const auto& [service, fingerprints] : serviceDatabase) {
        for (const auto& fingerprint : fingerprints) {
            std::regex signatureRegex(fingerprint.signature);
            if (std::regex_search(banner, signatureRegex)) {
                return fingerprint;
            }
        }
    }
    return {}; // No match found
}
