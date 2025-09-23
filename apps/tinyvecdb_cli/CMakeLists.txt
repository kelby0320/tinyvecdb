add_executable(tinyvecdb_cli src/main.cpp)

target_link_libraries(tinyvecdb_cli PRIVATE tinyvecdb::tinyvecdb)

install(TARGETS tinyvecdb_cli RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})