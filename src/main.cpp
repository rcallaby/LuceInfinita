#include "Logger.h"
#include "NetworkLayer.h"
#include "TaskScheduler.h"

int main(int argc, char* argv[]) {
    Logger logger("scan_logs.txt");
    NetworkLayer networkLayer(&logger);
    TaskScheduler scheduler(&logger);

    // Example task for TCP scan
    scheduler.addTask([&]() {
        networkLayer.tcpScan("127.0.0.1", 1, 100);
    });

    // Example task for UDP scan
    scheduler.addTask([&]() {
        networkLayer.udpScan("127.0.0.1", 1, 100);
    });

    // Wait for tasks to complete (simple approach)
    std::this_thread::sleep_for(std::chrono::seconds(5));

    return 0;
}
