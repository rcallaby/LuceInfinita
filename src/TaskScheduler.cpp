#include "TaskScheduler.h"

TaskScheduler::TaskScheduler(Logger* logger, int numWorkers)
    : logger(logger), stop(false) {
    for (int i = 0; i < numWorkers; ++i) {
        workers.emplace_back(&TaskScheduler::workerThread, this);
    }
}

TaskScheduler::~TaskScheduler() {
    {
        std::unique_lock<std::mutex> lock(mtx);
        stop = true;
    }
    cv.notify_all();
    for (auto& worker : workers) {
        if (worker.joinable()) worker.join();
    }
}

void TaskScheduler::addTask(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(mtx);
        tasks.push(task);
    }
    cv.notify_one();
}

void TaskScheduler::workerThread() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this] { return !tasks.empty() || stop; });
            if (stop && tasks.empty()) return;
            task = std::move(tasks.front());
            tasks.pop();
        }
        task();
    }
}
