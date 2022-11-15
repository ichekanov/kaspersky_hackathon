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

# Utility rule file for gdbsim.

# Include any custom commands dependencies for this target.
include einit/CMakeFiles/gdbsim.dir/compiler_depend.make

# Include the progress variables for this target.
include einit/CMakeFiles/gdbsim.dir/progress.make

einit/CMakeFiles/gdbsim: einit/.gdbinit.ide
einit/CMakeFiles/gdbsim: einit/.gdbinit
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/kirill/kaspersky_hackathon/hello/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Running QEMU with KasperskyOS image..."
	cd /home/kirill/kaspersky_hackathon/hello/build/einit && /opt/KasperskyOS-Community-Edition-1.1.0.356/toolchain/bin/qemu-system-aarch64 -m 2048 -machine vexpress-a15,secure=on -cpu cortex-a72 -nographic -monitor none -smp 4 -nic user -serial stdio -serial tcp::12345,server,nowait -s -S -kernel /home/kirill/kaspersky_hackathon/hello/build/einit/kos-qemu-image

gdbsim: einit/CMakeFiles/gdbsim
gdbsim: einit/CMakeFiles/gdbsim.dir/build.make
.PHONY : gdbsim

# Rule to build all files generated by this target.
einit/CMakeFiles/gdbsim.dir/build: gdbsim
.PHONY : einit/CMakeFiles/gdbsim.dir/build

einit/CMakeFiles/gdbsim.dir/clean:
	cd /home/kirill/kaspersky_hackathon/hello/build/einit && $(CMAKE_COMMAND) -P CMakeFiles/gdbsim.dir/cmake_clean.cmake
.PHONY : einit/CMakeFiles/gdbsim.dir/clean

einit/CMakeFiles/gdbsim.dir/depend:
	cd /home/kirill/kaspersky_hackathon/hello/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kirill/kaspersky_hackathon/hello /home/kirill/kaspersky_hackathon/hello/einit /home/kirill/kaspersky_hackathon/hello/build /home/kirill/kaspersky_hackathon/hello/build/einit /home/kirill/kaspersky_hackathon/hello/build/einit/CMakeFiles/gdbsim.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : einit/CMakeFiles/gdbsim.dir/depend

