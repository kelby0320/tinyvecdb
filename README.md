## Build instructions

```bash
cmake -S . -B build
cmake --build build -j
ctest --test-dir build --output-on-failure