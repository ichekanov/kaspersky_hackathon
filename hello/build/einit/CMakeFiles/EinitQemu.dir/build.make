# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/KasperskyOS-Community-Edition-1.1.0.356/toolchain/bin/cmake

# The command to remove a file.
RM = /opt/KasperskyOS-Community-Edition-1.1.0.356/toolchain/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/kirill/kaspersky_hackathon/hello

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kirill/kaspersky_hackathon/hello/build

# Include any dependencies generated for this target.
include einit/CMakeFiles/EinitQemu.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include einit/CMakeFiles/EinitQemu.dir/compiler_depend.make

# Include the progress variables for this target.
include einit/CMakeFiles/EinitQemu.dir/progress.make

# Include the compile flags for this target's objects.
include einit/CMakeFiles/EinitQemu.dir/flags.make

einit/EinitQemu-kss/EinitQemu.c: einit/EinitQemu-kss/init.yaml
einit/EinitQemu-kss/EinitQemu.c: hello/Hello
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/kirill/kaspersky_hackathon/hello/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating EinitQemu-kss/EinitQemu.c"
	cd /home/kirill/kaspersky_hackathon/hello/build/einit && /opt/KasperskyOS-Community-Edition-1.1.0.356/toolchain/bin/einit -I /home/kirill/kaspersky_hackathon/hello/build/_headers_ -I /home/kirill/kaspersky_hackathon/hello/build/_headers_/hello/.. -I /home/kirill/kaspersky_hackathon/hello/build/../install/include -I /opt/KasperskyOS-Community-Edition-1.1.0.356/sysroot-aarch64-kos/include -I /opt/KasperskyOS-Community-Edition-1.1.0.356/toolchain/aarch64-kos/include -I /sysroot-aarch64-kos/include -I /opt/KasperskyOS-Community-Edition-1.1.0.356/toolchain/include -o /home/kirill/kaspersky_hackathon/hello/build/einit/EinitQemu-kss/EinitQemu.c /home/kirill/kaspersky_hackathon/hello/build/einit/EinitQemu-kss/init.yaml

einit/CMakeFiles/EinitQemu.dir/EinitQemu-kss/EinitQemu.c.o: einit/CMakeFiles/EinitQemu.dir/flags.make
einit/CMakeFiles/EinitQemu.dir/EinitQemu-kss/EinitQemu.c.o: einit/EinitQemu-kss/EinitQemu.c
einit/CMakeFiles/EinitQemu.dir/EinitQemu-kss/EinitQemu.c.o: einit/CMakeFiles/EinitQemu.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kirill/kaspersky_hackathon/hello/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object einit/CMakeFiles/EinitQemu.dir/EinitQemu-kss/EinitQemu.c.o"
	cd /home/kirill/kaspersky_hackathon/hello/build/einit && /opt/KasperskyOS-Community-Edition-1.1.0.356/toolchain/bin/aarch64-kos-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT einit/CMakeFiles/EinitQemu.dir/EinitQemu-kss/EinitQemu.c.o -MF CMakeFiles/EinitQemu.dir/EinitQemu-kss/EinitQemu.c.o.d -o CMakeFiles/EinitQemu.dir/EinitQemu-kss/EinitQemu.c.o -c /home/kirill/kaspersky_hackathon/hello/build/einit/EinitQemu-kss/EinitQemu.c

einit/CMakeFiles/EinitQemu.dir/EinitQemu-kss/EinitQemu.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/EinitQemu.dir/EinitQemu-kss/EinitQemu.c.i"
	cd /home/kirill/kaspersky_hackathon/hello/build/einit && /opt/KasperskyOS-Community-Edition-1.1.0.356/toolchain/bin/aarch64-kos-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kirill/kaspersky_hackathon/hello/build/einit/EinitQemu-kss/EinitQemu.c > CMakeFiles/EinitQemu.dir/EinitQemu-kss/EinitQemu.c.i

einit/CMakeFiles/EinitQemu.dir/EinitQemu-kss/EinitQemu.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/EinitQemu.dir/EinitQemu-kss/EinitQemu.c.s"
	cd /home/kirill/kaspersky_hackathon/hello/build/einit && /opt/KasperskyOS-Community-Edition-1.1.0.356/toolchain/bin/aarch64-kos-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kirill/kaspersky_hackathon/hello/build/einit/EinitQemu-kss/EinitQemu.c -o CMakeFiles/EinitQemu.dir/EinitQemu-kss/EinitQemu.c.s

# Object files for target EinitQemu
EinitQemu_OBJECTS = \
"CMakeFiles/EinitQemu.dir/EinitQemu-kss/EinitQemu.c.o"

# External object files for target EinitQemu
EinitQemu_EXTERNAL_OBJECTS =

einit/EinitQemu: einit/CMakeFiles/EinitQemu.dir/EinitQemu-kss/EinitQemu.c.o
einit/EinitQemu: einit/CMakeFiles/EinitQemu.dir/build.make
einit/EinitQemu: einit/CMakeFiles/EinitQemu.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kirill/kaspersky_hackathon/hello/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable EinitQemu"
	cd /home/kirill/kaspersky_hackathon/hello/build/einit && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/EinitQemu.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
einit/CMakeFiles/EinitQemu.dir/build: einit/EinitQemu
.PHONY : einit/CMakeFiles/EinitQemu.dir/build

einit/CMakeFiles/EinitQemu.dir/clean:
	cd /home/kirill/kaspersky_hackathon/hello/build/einit && $(CMAKE_COMMAND) -P CMakeFiles/EinitQemu.dir/cmake_clean.cmake
.PHONY : einit/CMakeFiles/EinitQemu.dir/clean

einit/CMakeFiles/EinitQemu.dir/depend: einit/EinitQemu-kss/EinitQemu.c
	cd /home/kirill/kaspersky_hackathon/hello/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kirill/kaspersky_hackathon/hello /home/kirill/kaspersky_hackathon/hello/einit /home/kirill/kaspersky_hackathon/hello/build /home/kirill/kaspersky_hackathon/hello/build/einit /home/kirill/kaspersky_hackathon/hello/build/einit/CMakeFiles/EinitQemu.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : einit/CMakeFiles/EinitQemu.dir/depend

