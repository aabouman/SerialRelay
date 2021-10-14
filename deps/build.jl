using CMake

builddir = joinpath(@__DIR__, "build")
if !isdir(builddir)
    Base.Filesystem.mkdir(builddir)
end

debug = true
# Debug string to compile the library with debug flags
debug_str = "-DCMAKE_BUILD_TYPE=Release"
if debug
    debug_str = "-DCMAKE_BUILD_TYPE=Debug"
end

serial_relay_lib_dir = joinpath(@__DIR__, "src")

config_cmd = `$cmake -S$serial_relay_lib_dir $debug_str -B$builddir`
build_cmd = `$cmake --build $builddir`

run(config_cmd)
run(build_cmd)
