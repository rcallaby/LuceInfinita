#ifndef SERVICE_ENUMERATION_HPP
#define SERVICE_ENUMERATION_HPP

#include <string>
#include <map>
#include <vector>

struct ServiceFingerprint {
    std::string name;
    std::string version;
    std::string signature;  // A simple regex or string pattern for matching
};

class ServiceEnumeration {
public:
    ServiceEnumeration(const std::string& databaseFile);
    std::vector<ServiceFingerprint> enumerateService(const std::string& ip, int port);

private:
    std::map<std::string, std::vector<ServiceFingerprint>> serviceDatabase;
    void loadDatabase(const std::string& databaseFile);
    std::string grabBanner(const std::string& ip, int port);
    ServiceFingerprint matchService(const std::string& banner);
};

#endif // SERVICE_ENUMERATION_HPP
