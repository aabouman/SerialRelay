# Import correct library name for specific system
libhg_library_filename = ""
if Sys.islinux()
    libhg_library_filename = joinpath(@__DIR__, "..", "deps", "build", "libhg.so")
elseif Sys.isapple()
    libhg_library_filename = joinpath(@__DIR__, "..", "deps", "build", "libhg.dylib")
end
const libhg = libhg_library_filename


function serial_relay_launch(port_name,
                             baudrate,
                             sub_endpoint,
                             pub_endpoint)
    ccall((:relay_launch, libhg), Cvoid, (Cstring, Cint, Cstring, Cstring), us)
end
