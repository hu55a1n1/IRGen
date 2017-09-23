# IRGen
Arduino-IRremote stripped of its Arduino dependencies.

`IRGen` runs on a desktop computer and simply generates raw IR codes for supported devices, printing them to `stdout`.

## Build
Build using the provided Makefile:
```sh
make all
```

Or use cmake:
```sh
mkdir -p cmake-build-debug
cd cmake-build-debug
cmake ..
make
```

This should build all demos in the `cmake-build-debug` directory.

Tested on clang (Mac OSX 10.12.6) and g++ (Ubuntu 16.04).
