# Prerequisites
- CCache
- CMake
- both of these are available from homebrew

# Setup
`cmake CMakeLists.txt`

This command will use cmake to build the make file for the project

# Building
`make all`

This command will compile source in `./src` and will use ccache to
incrementally compile the source after the first full build

make will produce a linked executable in the `./build` directory

# Other useful commands
- `leaks -atExit -- ./build/atest`
This command will run emory leak detection on the executable and will also
share basic infomraiton about memory allocations.

- `time make all && ./build/atest ./data/df.txn`
This command will incrementally build changes and immediately run the output
file with the supplied parameter as input
