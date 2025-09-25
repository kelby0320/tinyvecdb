# Use GoogleTest via FetchContent to avoid external deps
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

function(tinyvecdb_add_test name)
  add_executable(${name} ${ARGN})
  target_include_directories(${name} PRIVATE ${TINYVECDB_PUBLIC_INCLUDE_DIR})
  target_compile_features(${name} PRIVATE cxx_std_20)
  target_link_libraries(${name}
    PRIVATE
      GTest::gtest GTest::gtest_main
      tinyvecdb_core
      # tinyvecdb_service         # if tests touch API guard
      # tinyvecdb_query_engine
      # tinyvecdb_vector_store
      # tinyvecdb_kernels
      # tinyvecdb_utils
      # tinyvecdb_persistence
      # tinyvecdb_index_hnsw
  )
  add_test(NAME ${name} COMMAND ${name})
endfunction()

# Unit tests (add or remove as files land)
tinyvecdb_add_test(test_smoke             basic_smoke.cpp)
# tinyvecdb_add_test(test_distance        unit/test_distance.cc)
# tinyvecdb_add_test(test_topk            unit/test_topk.cc)
# tinyvecdb_add_test(test_vector_store    unit/test_vector_store.cc)
# tinyvecdb_add_test(test_hnsw_basic      unit/test_hnsw_basic.cc)
# tinyvecdb_add_test(test_query_flat      unit/test_query_engine_flat.cc)
# tinyvecdb_add_test(test_epoch_reclaim   unit/test_epoch_reclamation.cc)
# tinyvecdb_add_test(test_wal_recovery    unit/test_wal_recovery.cc)
# tinyvecdb_add_test(test_namespace       unit/test_namespace.cc)
