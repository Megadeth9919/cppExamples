# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_SOURCE_DIR = /home/qiuyuang/cppExamples

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/qiuyuang/cppExamples/build

# Include any dependencies generated for this target.
include others/CMakeFiles/CRTP2.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include others/CMakeFiles/CRTP2.dir/compiler_depend.make

# Include the progress variables for this target.
include others/CMakeFiles/CRTP2.dir/progress.make

# Include the compile flags for this target's objects.
include others/CMakeFiles/CRTP2.dir/flags.make

others/CMakeFiles/CRTP2.dir/CRTP2.cpp.o: others/CMakeFiles/CRTP2.dir/flags.make
others/CMakeFiles/CRTP2.dir/CRTP2.cpp.o: /home/qiuyuang/cppExamples/others/CRTP2.cpp
others/CMakeFiles/CRTP2.dir/CRTP2.cpp.o: others/CMakeFiles/CRTP2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/qiuyuang/cppExamples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object others/CMakeFiles/CRTP2.dir/CRTP2.cpp.o"
	cd /home/qiuyuang/cppExamples/build/others && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT others/CMakeFiles/CRTP2.dir/CRTP2.cpp.o -MF CMakeFiles/CRTP2.dir/CRTP2.cpp.o.d -o CMakeFiles/CRTP2.dir/CRTP2.cpp.o -c /home/qiuyuang/cppExamples/others/CRTP2.cpp

others/CMakeFiles/CRTP2.dir/CRTP2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/CRTP2.dir/CRTP2.cpp.i"
	cd /home/qiuyuang/cppExamples/build/others && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/qiuyuang/cppExamples/others/CRTP2.cpp > CMakeFiles/CRTP2.dir/CRTP2.cpp.i

others/CMakeFiles/CRTP2.dir/CRTP2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/CRTP2.dir/CRTP2.cpp.s"
	cd /home/qiuyuang/cppExamples/build/others && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/qiuyuang/cppExamples/others/CRTP2.cpp -o CMakeFiles/CRTP2.dir/CRTP2.cpp.s

# Object files for target CRTP2
CRTP2_OBJECTS = \
"CMakeFiles/CRTP2.dir/CRTP2.cpp.o"

# External object files for target CRTP2
CRTP2_EXTERNAL_OBJECTS =

others/CRTP2: others/CMakeFiles/CRTP2.dir/CRTP2.cpp.o
others/CRTP2: others/CMakeFiles/CRTP2.dir/build.make
others/CRTP2: others/CMakeFiles/CRTP2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/qiuyuang/cppExamples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable CRTP2"
	cd /home/qiuyuang/cppExamples/build/others && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CRTP2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
others/CMakeFiles/CRTP2.dir/build: others/CRTP2
.PHONY : others/CMakeFiles/CRTP2.dir/build

others/CMakeFiles/CRTP2.dir/clean:
	cd /home/qiuyuang/cppExamples/build/others && $(CMAKE_COMMAND) -P CMakeFiles/CRTP2.dir/cmake_clean.cmake
.PHONY : others/CMakeFiles/CRTP2.dir/clean

others/CMakeFiles/CRTP2.dir/depend:
	cd /home/qiuyuang/cppExamples/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/qiuyuang/cppExamples /home/qiuyuang/cppExamples/others /home/qiuyuang/cppExamples/build /home/qiuyuang/cppExamples/build/others /home/qiuyuang/cppExamples/build/others/CMakeFiles/CRTP2.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : others/CMakeFiles/CRTP2.dir/depend

