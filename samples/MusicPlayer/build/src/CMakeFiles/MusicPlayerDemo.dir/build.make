# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jh/code/enigma/samples/MusicPlayer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jh/code/enigma/samples/MusicPlayer/build

# Include any dependencies generated for this target.
include src/CMakeFiles/MusicPlayerDemo.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/MusicPlayerDemo.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/MusicPlayerDemo.dir/flags.make

src/CMakeFiles/MusicPlayerDemo.dir/musicplayer_main.cpp.o: src/CMakeFiles/MusicPlayerDemo.dir/flags.make
src/CMakeFiles/MusicPlayerDemo.dir/musicplayer_main.cpp.o: ../src/musicplayer_main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jh/code/enigma/samples/MusicPlayer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/MusicPlayerDemo.dir/musicplayer_main.cpp.o"
	cd /home/jh/code/enigma/samples/MusicPlayer/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MusicPlayerDemo.dir/musicplayer_main.cpp.o -c /home/jh/code/enigma/samples/MusicPlayer/src/musicplayer_main.cpp

src/CMakeFiles/MusicPlayerDemo.dir/musicplayer_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MusicPlayerDemo.dir/musicplayer_main.cpp.i"
	cd /home/jh/code/enigma/samples/MusicPlayer/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jh/code/enigma/samples/MusicPlayer/src/musicplayer_main.cpp > CMakeFiles/MusicPlayerDemo.dir/musicplayer_main.cpp.i

src/CMakeFiles/MusicPlayerDemo.dir/musicplayer_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MusicPlayerDemo.dir/musicplayer_main.cpp.s"
	cd /home/jh/code/enigma/samples/MusicPlayer/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jh/code/enigma/samples/MusicPlayer/src/musicplayer_main.cpp -o CMakeFiles/MusicPlayerDemo.dir/musicplayer_main.cpp.s

# Object files for target MusicPlayerDemo
MusicPlayerDemo_OBJECTS = \
"CMakeFiles/MusicPlayerDemo.dir/musicplayer_main.cpp.o"

# External object files for target MusicPlayerDemo
MusicPlayerDemo_EXTERNAL_OBJECTS =

../bin/MusicPlayerDemo: src/CMakeFiles/MusicPlayerDemo.dir/musicplayer_main.cpp.o
../bin/MusicPlayerDemo: src/CMakeFiles/MusicPlayerDemo.dir/build.make
../bin/MusicPlayerDemo: src/CMakeFiles/MusicPlayerDemo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jh/code/enigma/samples/MusicPlayer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/MusicPlayerDemo"
	cd /home/jh/code/enigma/samples/MusicPlayer/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MusicPlayerDemo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/MusicPlayerDemo.dir/build: ../bin/MusicPlayerDemo

.PHONY : src/CMakeFiles/MusicPlayerDemo.dir/build

src/CMakeFiles/MusicPlayerDemo.dir/clean:
	cd /home/jh/code/enigma/samples/MusicPlayer/build/src && $(CMAKE_COMMAND) -P CMakeFiles/MusicPlayerDemo.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/MusicPlayerDemo.dir/clean

src/CMakeFiles/MusicPlayerDemo.dir/depend:
	cd /home/jh/code/enigma/samples/MusicPlayer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jh/code/enigma/samples/MusicPlayer /home/jh/code/enigma/samples/MusicPlayer/src /home/jh/code/enigma/samples/MusicPlayer/build /home/jh/code/enigma/samples/MusicPlayer/build/src /home/jh/code/enigma/samples/MusicPlayer/build/src/CMakeFiles/MusicPlayerDemo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/MusicPlayerDemo.dir/depend
