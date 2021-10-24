#pragma once

#include <iostream>
#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic>


/** 
 * @class BlockingQueue.
 * thread safe queue, when there is no Data left the thread will be sleep
 **/
template <typename DataType>
class BlockingQueue {
public:
    /**
    * put task in the queue.
    * if there is thread waiting for the task, will wake up one thread.
    * @param data
    * @return 0 if ok, other wise -1
    */
    int enqueue(const DataType& data) {
        std::unique_lock<std::mutex> lock(_lock);
        //if the queue if full 
        _cond.wait(lock, [this](){_task_queue.size() > _max_size});
        _task_queue.push(data);
        _cond.notify_one();
        return true;
    }

    /**
    * put task in the queue.(move version)
    * if there is thread waiting for the task, will wake up one thread.
    * @param data
    * @return 0 if ok, other wise -1
    */
    int enqueue(const DataType&& data) {
        std::unique_lock<std::mutex> lock(_lock);
        //if the queue if full 
        _cond.wait(lock, [this](){_task_queue.size() > _max_size});
        _task_queue.emplace(std::move(data));
        _cond.notify_one();
        return true;
    } 

    /**
    * get data from the queue. if the queue is empty, the thread will blocking(sleep) till one task occured.
    * @return data. 
    */
    DataType get_data() {
        std::unique_lock<std::mutex> lock(_lock);
        if (_task_queue.empty()) {
            _cond.wait();
        }
        auto result = _task_queue.front();
        _task_queue.pop();
        return result;
    }

   
private:
    /* task queue. */
    std::queue<DataType>                _task_queue;

    /* lock. */
    std::mutex                          _lock;
    
    /* queue size. */
    int64_t                             _max_size;

    /* conditional variable. */
    std::condition_variable             _cond;

    /* atomic task. */
    std::atomic<int64_t>                _task_cnt;
};