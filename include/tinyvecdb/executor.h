#pragma once

#include <cstdint>
#include <functional>
#include <future>
#include <string>
#include <type_traits>

namespace tinyvecdb {

enum class TaskPriority { LOW, NORMAL, HIGH };

struct TaskClass {
    std::string name;
    TaskPriority priority{TaskPriority::NORMAL};
    uint16_t max_concurrency{0}; // 0 = no limit
};

class IExecutor {
  public:
    virtual ~IExecutor() = default;

    template <typename Fn, typename... Args>
    auto submit(const TaskClass &tc, Fn &&fn, Args &&...args)
        -> std::future<std::invoke_result_t<Fn, Args...>>;

    // // Fire and forget (no future returned)
    // virtual void post(const TaskClass &tc, std::function<void()> fn) = 0;

    // // Schedule a task to run after a delay
    // virtual void schedule_after(std::chrono::milliseconds delay,
    //                             const TaskClass &tc,
    //                             std::function<void()> fn) = 0;

  private:
    virtual std::future<void> submit_impl(const TaskClass &tc,
                                          std::packaged_task<void()> task) = 0;
};

template <typename Fn, typename... Args>
auto IExecutor::submit(const TaskClass &tc, Fn &&fn, Args &&...args)
    -> std::future<std::invoke_result_t<Fn, Args...>> {
    using result_t = std::invoke_result_t<Fn, Args...>;
    std::packaged_task<result_t()> task(
        std::bind(std::forward<Fn>(fn), std::forward<Args>(args)...));

    auto fut = task.get_future();

    auto erased = std::packaged_task<void()>(
        [task = std::move(task)]() mutable { task(); });

    submit_impl(tc, std::move(erased));
    return fut;
}

inline const TaskClass kQueryCpu{"QueryCpu", TaskPriority::HIGH, 0};
inline const TaskClass kBuildIndex{"BuildIndex", TaskPriority::NORMAL, 0};
inline const TaskClass kPersistWAL{"PersistWAL", TaskPriority::HIGH, 1};
inline const TaskClass kBgCompact{"BgCompact", TaskPriority::LOW, 1};

} // namespace tinyvecdb