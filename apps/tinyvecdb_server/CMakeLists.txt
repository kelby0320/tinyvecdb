add_executable(tinyvecdb_server src/main.cpp)

target_link_libraries(tinyvecdb_server PRIVATE tinyvecdb::tinyvecdb)

install(TARGETS tinyvecdb_server RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})