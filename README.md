To build, run these steps:
```
mkdir build
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -j8
```

The first 2 create the build folder and move inside it. The 3rd configures the
build system with the Ninja generator (usually faster and better than Unix Makefiles,
you can leave it off to use Makefiles), sets the build type to Release (full optimization,
no debug info), and calls it on the root directory. The last step invokes the build
command for the particular build system (Ninja in this case, `make` by default), and
builds with 8 threads.

The executable for this example is directly inside the `build` folder, and label `zmq_example`.
