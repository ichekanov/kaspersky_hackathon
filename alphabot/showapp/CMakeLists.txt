cmake_minimum_required(VERSION 3.12)

# Toolchain for working with the NK-parser.
include (platform/nk)

# Set compiler flags.
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -Wno-deprecated-declarations")

add_executable (Showapp "src/showapp.cpp" "src/Motor.hpp")
add_dependencies (Showapp showapp_edl_files)

target_link_libraries (Showapp ${gpio_CLIENT_LIB})

target_link_libraries (Showapp ${bsp_CLIENT_LIB})

set_target_properties (Showapp PROPERTIES LINK_FLAGS ${VULN_LDFLAGS})

