| Command                                               | What it does                                                                                |
| ----------------------------------------------------- | ------------------------------------------------------------------------------------------- |
| `mkdir -p build && cd build && cmake ..`              | generate the `build` directory which holds all generated files such as executables          |
| `rm -rf build && mkdir build && cd build && cmake ..` | remove and regenerate the `build` directory                                                 |
| `make`                                                | compile all executables (all make commands are to be run from within the `build` directory) |
| `make && make test`                                   | compile all executables and run all your tests                                              |
| `make cov`                                            | generate a code coverage report that can be found under `build/code_coverage/report`        |
| `make format`                                         | auto format your code                                                                       |
| `make cppcheck`                                       | check your code for possible bugs                                                           |
| `make scan-build`                                     | check your code for possible bugs                                                           |
| `valgrind build/test/bst/test_BST.cpp.executable`     | analyze a single executable for memory leaks                                                |
| `gdb build/test/bst/test_BST.cpp.executable`          | debug a file                                                                                |