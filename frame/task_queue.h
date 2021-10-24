#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <memory>
#include <functional>
#include <utility>
#include <unistd.h>


template <typename TaskType>
class TaskQueue {
public:
    int enqueue(const TaskType &task) {
        std::unique_lock<std::mutex> lock(_lock);
        _task_queue.push(task);
        _cond.notify_one();
        return 0;
    }

    TaskType get_task() {
        std::unique_lock<std::mutex> lock(_lock);
        if (_task_queue.empty()) {
            _cond.wait(lock);
        }
        auto result = _task_queue.front();
        _task_queue.pop();
        return result;
    }


// only show how to user the task_queue
void put_task() {
    int cnt = 1;
    while (is_running) {
        TaskType task(cnt);
        enqueue(task);
        ++cnt;
        sleep(1);
    }
    
}

void run_task() {
    while(is_running) {
        TaskType task = get_task();
        task.run();
    }
}

    bool is_running = false;
private:
    std::queue<TaskType> _task_queue;
    std::mutex           _lock;
    std::condition_variable _cond;

};

class Task{
public:
    int val;
    Task(const int x) : val(x) {};
    int run() {
        std::cout << val << std::endl;
        return 0;
    }
    
};