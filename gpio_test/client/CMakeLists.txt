project (client)

# Toolchain for working with the NK-parser.
include (platform/nk)

nk_build_edl_files (client_edl_files NK_MODULE "client" EDL "${CMAKE_SOURCE_DIR}/resources/edl/Client.edl")

add_executable (Client "src/main.cpp" "src/Motor.h")
add_dependencies (Client client_edl_files)
target_link_libraries (Client ${gpio_CLIENT_LIB})

if ("${CMAKE_SYSTEM_PROCESSOR}" MATCHES "arm|aarch64")
    target_link_libraries (Client ${bsp_CLIENT_LIB})
endif ()

set_target_properties (Client PROPERTIES LINK_FLAGS ${VULN_LDFLAGS})
