#include "shared_thread_pool.h"

namespace tinyvecdb {

// Define static thread_local member variables
thread_local tinyvecdb::SharedThreadPool::TaskQueue
    *SharedThreadPool::local_queue_ = nullptr;
thread_local size_t tinyvecdb::SharedThreadPool::local_index_ = 0;

} // namespace tinyvecdb

tinyvecdb::SharedThreadPool::SharedThreadPool(size_t num_threads)
    : threads_(num_threads) {
    if (threads_ == 0) {
        threads_ = 1;
    }

    for (size_t i = 0; i < threads_; i++) {
        queues_.emplace_back(std::make_unique<TaskQueue>());
    }

    for (size_t i = 0; i < threads_; i++) {
        workers_.emplace_back(&SharedThreadPool::worker_loop, this, i);
    }
}

tinyvecdb::SharedThreadPool::~SharedThreadPool() {
    stopped_ = true;
    for (auto &worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void tinyvecdb::SharedThreadPool::worker_loop(size_t index) {
    local_queue_ = queues_[index].get();
    local_index_ = index;

    while (!stopped_) {
        run_pending_task();
    }
}

void tinyvecdb::SharedThreadPool::run_pending_task() {
    Task item;
    if (pop_task_from_local(item) || pop_task_from_global(item) ||
        steal_task(item)) {
        item.task();
    } else {
        std::this_thread::yield();
    }
}

bool tinyvecdb::SharedThreadPool::pop_task_from_local(
    SharedThreadPool::Task &item) {
    return local_queue_ && local_queue_->try_pop(item);
}

bool tinyvecdb::SharedThreadPool::pop_task_from_global(
    SharedThreadPool::Task &item) {
    return g_queue_.try_pop(item);
}

bool tinyvecdb::SharedThreadPool::steal_task(SharedThreadPool::Task &item) {
    for (size_t i = 0; i < queues_.size(); i++) {
        const size_t index = (local_index_ + i + 1) % queues_.size();
        if (queues_[i]->try_steal(item)) {
            return true;
        }
    }
    return false;
}

std::future<void>
tinyvecdb::SharedThreadPool::submit_impl(const TaskClass &tc,
                                         std::packaged_task<void()> task) {
    auto fut = task.get_future();

    if (local_queue_) {
        local_queue_->push({tc, std::move(task)});
    } else {
        g_queue_.push({tc, std::move(task)});
    }

    return fut;
}

void tinyvecdb::SharedThreadPool::TaskQueue::push(SharedThreadPool::Task item) {
    std::lock_guard<std::mutex> lock(mu_);
    switch (item.tc.priority) {
    case TaskPriority::HIGH:
        high.push_back(std::move(item));
        break;
    case TaskPriority::NORMAL:
        normal.push_back(std::move(item));
        break;
    case TaskPriority::LOW:
        low.push_back(std::move(item));
        break;
    }
}

bool tinyvecdb::SharedThreadPool::TaskQueue::empty() const {
    std::lock_guard<std::mutex> lock(mu_);
    return high.empty() && normal.empty() && low.empty();
}

bool tinyvecdb::SharedThreadPool::TaskQueue::try_pop(
    SharedThreadPool::Task &item) {
    std::lock_guard<std::mutex> lock(mu_);
    if (!high.empty()) {
        item = std::move(high.front());
        high.pop_front();
        return true;
    }
    if (!normal.empty()) {
        item = std::move(normal.front());
        normal.pop_front();
        return true;
    }
    if (!low.empty()) {
        item = std::move(low.front());
        low.pop_front();
        return true;
    }
    return false;
}

bool tinyvecdb::SharedThreadPool::TaskQueue::try_steal(
    SharedThreadPool::Task &item) {
    std::lock_guard<std::mutex> lock(mu_);
    if (!high.empty()) {
        item = std::move(high.back());
        high.pop_back();
        return true;
    }
    if (!normal.empty()) {
        item = std::move(normal.back());
        normal.pop_back();
        return true;
    }
    if (!low.empty()) {
        item = std::move(low.back());
        low.pop_back();
        return true;
    }
    return false;
}