#include <gtest/gtest.h>

#include "shared_thread_pool.h"
#include "tinyvecdb/executor.h"

#include <future>
#include <string>

TEST(SharedThreadPool, SubmitTasks) {
    tinyvecdb::SharedThreadPool pool(4);

    auto fut1 = pool.submit(tinyvecdb::kQueryCpu, []() { return 42; });
    auto fut2 = pool.submit(tinyvecdb::kQueryCpu,
                            []() { return std::string("hello"); });
    auto fut3 = pool.submit(tinyvecdb::kQueryCpu, []() { return 3.14; });

    EXPECT_EQ(fut1.get(), 42);
    EXPECT_EQ(fut2.get(), "hello");
    EXPECT_DOUBLE_EQ(fut3.get(), 3.14);
}