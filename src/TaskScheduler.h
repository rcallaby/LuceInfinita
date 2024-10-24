#ifndef TASK_SCHEDULER_H
#define TASK_SCHEDULER_H

#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include "Logger.h"

class TaskScheduler {
private:
    Logger* logger;
    std::queue<std::function<void()>> tasks;
    std::vector<std::thread> workers;
    std::mutex mtx;
    std::condition_variable cv;
    bool stop;

public:
    TaskScheduler(Logger* logger, int numWorkers = 4);
    ~TaskScheduler();

    void addTask(std::function<void()> task);
    void workerThread();
};

#endif // TASK_SCHEDULER_H
