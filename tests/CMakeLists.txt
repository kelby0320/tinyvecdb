include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
# For MSVC runtime behavior
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(tinyvecdb_tests basic_smoke.cpp)

target_link_libraries(tinyvecdb_tests PRIVATE
  tinyvecdb::tinyvecdb
  GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tinyvecdb_tests)