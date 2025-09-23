cmake_minimum_required(VERSION 3.21)

project(tinyvecdb
VERSION 0.1.0
DESCRIPTION "Tiny Vector Database (MVP)"
LANGUAGES CXX)

# Options
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(TINYVECDB_ENABLE_HNSW "Enable optional HNSW index support" OFF)
option(TINYVECDB_BUILD_TESTS "Build tests" ON)
option(TINYVECDB_BUILD_APPS "Build CLI/apps" ON)

# Global settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# MSVC warnings as errors toggle (opt‑in)
option(TINYVECDB_STRICT_WARNINGS "Treat warnings as errors" OFF)

# Include dirs
include(GNUInstallDirs)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Library target
add_library(tinyvecdb
src/tinyvecdb.cpp
src/vector_store.cpp
# src/query_engine.cpp
)

add_library(tinyvecdb::tinyvecdb ALIAS tinyvecdb)

target_include_directories(tinyvecdb
PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# Public compile defs and features
if(TINYVECDB_ENABLE_HNSW)
target_compile_definitions(tinyvecdb PUBLIC TINYVECDB_ENABLE_HNSW=1)
endif()

# Warnings
if(MSVC)
target_compile_options(tinyvecdb PRIVATE /W4)
if(TINYVECDB_STRICT_WARNINGS)
target_compile_options(tinyvecdb PRIVATE /WX)
endif()
else()
target_compile_options(tinyvecdb PRIVATE -Wall -Wextra -Wpedantic)
if(TINYVECDB_STRICT_WARNINGS)
target_compile_options(tinyvecdb PRIVATE -Werror)
endif()
endif()

# Interface headers are the public API
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Export and install the library
install(TARGETS tinyvecdb
EXPORT tinyvecdbTargets
ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Generate and install package config
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
"${CMAKE_CURRENT_BINARY_DIR}/tinyvecdbConfigVersion.cmake"
VERSION ${PROJECT_VERSION}
COMPATIBILITY SameMajorVersion)

configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/cmake/tinyvecdbConfig.cmake.in
${CMAKE_CURRENT_BINARY_DIR}/tinyvecdbConfig.cmake
@ONLY)

install(FILES
"${CMAKE_CURRENT_BINARY_DIR}/tinyvecdbConfig.cmake"
"${CMAKE_CURRENT_BINARY_DIR}/tinyvecdbConfigVersion.cmake"
DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tinyvecdb)

install(EXPORT tinyvecdbTargets
NAMESPACE tinyvecdb::
DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tinyvecdb)

# Apps (CLI)
if(TINYVECDB_BUILD_APPS)
add_subdirectory(apps/tinyvecdb_cli)
add_subdirectory(apps/tinyvecdb_server)
endif()

# Tests
if(TINYVECDB_BUILD_TESTS)
include(CTest)
enable_testing()
add_subdirectory(tests)
endif()