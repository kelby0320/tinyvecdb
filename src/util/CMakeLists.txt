add_library(tinyvecdb_util STATIC
  shared_thread_pool.cpp
)

target_include_directories(tinyvecdb_util
  PUBLIC
    ${TINYVECDB_PUBLIC_INCLUDE_DIR}
    ${TINYVECDB_GENERATED_INC_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(tinyvecdb_util PUBLIC cxx_std_20)