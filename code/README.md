# How to build

If the build system `ninja` is available:

```shell
cmake -S . -B build -G"Ninja Multi-Config"
cmake --build build --config Debug
cmake --build build --config Release
```

Otherwise, just rely on the default generator (typically Makefiles):

```shell
cmake -S . -B build-d -DCMAKE_BUILD_TYPE=Debug
cmake --build build-d
```

```shell
cmake -S . -B build-o -DCMAKE_BUILD_TYPE=Release
cmake --build build-o
```

The build artifacts (e.g. executables) are in the build directory.
