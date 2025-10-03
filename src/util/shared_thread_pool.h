#pragma once

#include "tinyvecdb/executor.h"

#include <atomic>
#include <cstdint>
#include <deque>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace tinyvecdb {

class SharedThreadPool : public IExecutor {
  public:
    explicit SharedThreadPool(
        size_t num_threads = std::thread::hardware_concurrency());
    ~SharedThreadPool() override;

    // void post(const TaskClass &tc, std::function<void()> fn) override;
    // void schedule_after(std::chrono::milliseconds delay, const TaskClass &tc,
    //                     std::function<void()> fn) override;

  private:
    struct Task {
        TaskClass tc;
        std::packaged_task<void()> task;
    };

    class TaskQueue {
      public:
        TaskQueue() = default;
        ~TaskQueue() = default;

        TaskQueue(const TaskQueue &) = delete;
        TaskQueue &operator=(const TaskQueue &) = delete;
        TaskQueue(TaskQueue &&) = delete;
        TaskQueue &operator=(TaskQueue &&) = delete;

        void push(Task item);
        bool empty() const;
        bool try_pop(Task &item);
        bool try_steal(Task &item);

      private:
        std::deque<Task> high, normal, low;
        mutable std::mutex mu_;
    };

    std::vector<std::thread> workers_;
    std::vector<std::unique_ptr<TaskQueue>> queues_;

    TaskQueue g_queue_;

    size_t threads_;
    std::atomic_bool stopped_{false};
    static thread_local TaskQueue *local_queue_;
    static thread_local size_t local_index_;

    void worker_loop(size_t index);
    void run_pending_task();
    bool pop_task_from_local(Task &item);
    bool pop_task_from_global(Task &item);
    bool steal_task(Task &item);

    std::future<void> submit_impl(const TaskClass &tc,
                                  std::packaged_task<void()> task) override;
};

} // namespace tinyvecdb