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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/Cache/POKER_WS/POKER_SIMULATION_v004

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/Cache/POKER_WS/POKER_SIMULATION_v004/build

# Include any dependencies generated for this target.
include CMakeFiles/simulate.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/simulate.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/simulate.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/simulate.dir/flags.make

CMakeFiles/simulate.dir/src/main_game.cpp.o: CMakeFiles/simulate.dir/flags.make
CMakeFiles/simulate.dir/src/main_game.cpp.o: ../src/main_game.cpp
CMakeFiles/simulate.dir/src/main_game.cpp.o: CMakeFiles/simulate.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/Cache/POKER_WS/POKER_SIMULATION_v004/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/simulate.dir/src/main_game.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/simulate.dir/src/main_game.cpp.o -MF CMakeFiles/simulate.dir/src/main_game.cpp.o.d -o CMakeFiles/simulate.dir/src/main_game.cpp.o -c /mnt/Cache/POKER_WS/POKER_SIMULATION_v004/src/main_game.cpp

CMakeFiles/simulate.dir/src/main_game.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simulate.dir/src/main_game.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/Cache/POKER_WS/POKER_SIMULATION_v004/src/main_game.cpp > CMakeFiles/simulate.dir/src/main_game.cpp.i

CMakeFiles/simulate.dir/src/main_game.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simulate.dir/src/main_game.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/Cache/POKER_WS/POKER_SIMULATION_v004/src/main_game.cpp -o CMakeFiles/simulate.dir/src/main_game.cpp.s

# Object files for target simulate
simulate_OBJECTS = \
"CMakeFiles/simulate.dir/src/main_game.cpp.o"

# External object files for target simulate
simulate_EXTERNAL_OBJECTS =

simulate: CMakeFiles/simulate.dir/src/main_game.cpp.o
simulate: CMakeFiles/simulate.dir/build.make
simulate: lib/liblib.a
simulate: /usr/lib/gcc/x86_64-linux-gnu/11/libgomp.so
simulate: /usr/lib/x86_64-linux-gnu/libpthread.a
simulate: CMakeFiles/simulate.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/Cache/POKER_WS/POKER_SIMULATION_v004/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable simulate"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/simulate.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/simulate.dir/build: simulate
.PHONY : CMakeFiles/simulate.dir/build

CMakeFiles/simulate.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/simulate.dir/cmake_clean.cmake
.PHONY : CMakeFiles/simulate.dir/clean

CMakeFiles/simulate.dir/depend:
	cd /mnt/Cache/POKER_WS/POKER_SIMULATION_v004/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/Cache/POKER_WS/POKER_SIMULATION_v004 /mnt/Cache/POKER_WS/POKER_SIMULATION_v004 /mnt/Cache/POKER_WS/POKER_SIMULATION_v004/build /mnt/Cache/POKER_WS/POKER_SIMULATION_v004/build /mnt/Cache/POKER_WS/POKER_SIMULATION_v004/build/CMakeFiles/simulate.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/simulate.dir/depend

