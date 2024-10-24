#ifndef NETWORK_LAYER_H
#define NETWORK_LAYER_H

#include <string>
#include "Logger.h"

class NetworkLayer {
private:
    Logger* logger;

public:
    NetworkLayer(Logger* logger);

    void tcpScan(const std::string& target, int startPort, int endPort);
    void udpScan(const std::string& target, int startPort, int endPort);
};

#endif // NETWORK_LAYER_H
