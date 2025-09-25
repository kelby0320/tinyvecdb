cmake_minimum_required(VERSION 3.20)
project(tinyvecdb VERSION 0.1.0 LANGUAGES CXX)

# -------- Options --------
option(TINYVECDB_BUILD_CLI     "Build tinyvecdb CLI"              OFF)
option(TINYVECDB_BUILD_TESTS   "Build unit tests"                 ON)
option(TINYVECDB_BUILD_BENCH   "Build benchmarks"                 OFF)
option(TINYVECDB_ENABLE_IVF    "Enable IVF index (optional)"      OFF)
option(TINYVECDB_ENABLE_PQ     "Enable PQ index (optional)"       OFF)
option(TINYVECDB_ENABLE_SNAP   "Enable snapshotting (optional)"   OFF)

# SIMD toggles (you can refine per-platform later)
option(TINYVECDB_ENABLE_SSE2   "Enable SSE2 kernels"              OFF)
option(TINYVECDB_ENABLE_AVX2   "Enable AVX2 kernels"              OFF)
option(TINYVECDB_ENABLE_AVX512 "Enable AVX-512 kernels"           OFF)
option(TINYVECDB_ENABLE_NEON   "Enable NEON kernels"              OFF)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Global language level
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# (Optional) Warnings & compiler flags helpers
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake/tinyvecdb-warnings.cmake")
  include(tinyvecdb-warnings)
endif()
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake/tinyvecdb-compiler-flags.cmake")
  include(tinyvecdb-compiler-flags)
endif()

# Generate a header with the version numbers/strings
set(GENERATED_INC_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated/include")
file(MAKE_DIRECTORY "${GENERATED_INC_DIR}/tinyvecdb")

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.h.in
  ${GENERATED_INC_DIR}/tinyvecdb/version.h
  @ONLY
)

# Make the generated header visible to dependents of the target

# Public headers live here
set(TINYVECDB_PUBLIC_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Generated headers live here
set(TINYVECDB_GENERATED_INC_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated/include")

add_subdirectory(src)

if(TINYVECDB_BUILD_CLI)
  add_subdirectory(tools/cli)
endif()

if(TINYVECDB_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

if(TINYVECDB_BUILD_BENCH)
  add_subdirectory(bench)
endif()

# Installation (headers)
install(DIRECTORY "${TINYVECDB_PUBLIC_INCLUDE_DIR}/tinyvecdb"
        DESTINATION include
        FILES_MATCHING PATTERN "*.h")
