## Build instructions

```bash
cmake -S . -B build -DTINYVECDB_BUILD_TESTS=ON -DTINYVECDB_BUILD_APPS=ON
cmake --build build -j
ctest --test-dir build --output-on-failure