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
include virtual/CMakeFiles/multi_inherit.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include virtual/CMakeFiles/multi_inherit.dir/compiler_depend.make

# Include the progress variables for this target.
include virtual/CMakeFiles/multi_inherit.dir/progress.make

# Include the compile flags for this target's objects.
include virtual/CMakeFiles/multi_inherit.dir/flags.make

virtual/CMakeFiles/multi_inherit.dir/multi_inherit.cpp.o: virtual/CMakeFiles/multi_inherit.dir/flags.make
virtual/CMakeFiles/multi_inherit.dir/multi_inherit.cpp.o: /home/qiuyuang/cppExamples/virtual/multi_inherit.cpp
virtual/CMakeFiles/multi_inherit.dir/multi_inherit.cpp.o: virtual/CMakeFiles/multi_inherit.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/qiuyuang/cppExamples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object virtual/CMakeFiles/multi_inherit.dir/multi_inherit.cpp.o"
	cd /home/qiuyuang/cppExamples/build/virtual && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT virtual/CMakeFiles/multi_inherit.dir/multi_inherit.cpp.o -MF CMakeFiles/multi_inherit.dir/multi_inherit.cpp.o.d -o CMakeFiles/multi_inherit.dir/multi_inherit.cpp.o -c /home/qiuyuang/cppExamples/virtual/multi_inherit.cpp

virtual/CMakeFiles/multi_inherit.dir/multi_inherit.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/multi_inherit.dir/multi_inherit.cpp.i"
	cd /home/qiuyuang/cppExamples/build/virtual && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/qiuyuang/cppExamples/virtual/multi_inherit.cpp > CMakeFiles/multi_inherit.dir/multi_inherit.cpp.i

virtual/CMakeFiles/multi_inherit.dir/multi_inherit.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/multi_inherit.dir/multi_inherit.cpp.s"
	cd /home/qiuyuang/cppExamples/build/virtual && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/qiuyuang/cppExamples/virtual/multi_inherit.cpp -o CMakeFiles/multi_inherit.dir/multi_inherit.cpp.s

# Object files for target multi_inherit
multi_inherit_OBJECTS = \
"CMakeFiles/multi_inherit.dir/multi_inherit.cpp.o"

# External object files for target multi_inherit
multi_inherit_EXTERNAL_OBJECTS =

virtual/multi_inherit: virtual/CMakeFiles/multi_inherit.dir/multi_inherit.cpp.o
virtual/multi_inherit: virtual/CMakeFiles/multi_inherit.dir/build.make
virtual/multi_inherit: virtual/CMakeFiles/multi_inherit.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/qiuyuang/cppExamples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable multi_inherit"
	cd /home/qiuyuang/cppExamples/build/virtual && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/multi_inherit.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
virtual/CMakeFiles/multi_inherit.dir/build: virtual/multi_inherit
.PHONY : virtual/CMakeFiles/multi_inherit.dir/build

virtual/CMakeFiles/multi_inherit.dir/clean:
	cd /home/qiuyuang/cppExamples/build/virtual && $(CMAKE_COMMAND) -P CMakeFiles/multi_inherit.dir/cmake_clean.cmake
.PHONY : virtual/CMakeFiles/multi_inherit.dir/clean

virtual/CMakeFiles/multi_inherit.dir/depend:
	cd /home/qiuyuang/cppExamples/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/qiuyuang/cppExamples /home/qiuyuang/cppExamples/virtual /home/qiuyuang/cppExamples/build /home/qiuyuang/cppExamples/build/virtual /home/qiuyuang/cppExamples/build/virtual/CMakeFiles/multi_inherit.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : virtual/CMakeFiles/multi_inherit.dir/depend

