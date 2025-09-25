#include <gtest/gtest.h>

#include "tinyvecdb/tinyvecdb.h"
#include "tinyvecdb/version.h"

TEST(Smoke, CheckVersionStringNotNull) {
    ASSERT_TRUE(tinyvecdb::VERSION_STRING != nullptr);
}

TEST(Smoke, CreateAndDestroyTinyVecDbInstance) {
    { tinyvecdb::TinyVecDb db; }
    ASSERT_TRUE(true);
}