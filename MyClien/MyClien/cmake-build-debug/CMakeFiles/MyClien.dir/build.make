# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

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
CMAKE_COMMAND = /cygdrive/c/Users/Devon/.CLion2016.3/system/cygwin_cmake/bin/cmake.exe

# The command to remove a file.
RM = /cygdrive/c/Users/Devon/.CLion2016.3/system/cygwin_cmake/bin/cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /cygdrive/c/Users/Devon/CLionProjects/MyClien

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /cygdrive/c/Users/Devon/CLionProjects/MyClien/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/MyClien.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/MyClien.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MyClien.dir/flags.make

CMakeFiles/MyClien.dir/main.c.o: CMakeFiles/MyClien.dir/flags.make
CMakeFiles/MyClien.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/Users/Devon/CLionProjects/MyClien/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/MyClien.dir/main.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/MyClien.dir/main.c.o   -c /cygdrive/c/Users/Devon/CLionProjects/MyClien/main.c

CMakeFiles/MyClien.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/MyClien.dir/main.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/Users/Devon/CLionProjects/MyClien/main.c > CMakeFiles/MyClien.dir/main.c.i

CMakeFiles/MyClien.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/MyClien.dir/main.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/Users/Devon/CLionProjects/MyClien/main.c -o CMakeFiles/MyClien.dir/main.c.s

CMakeFiles/MyClien.dir/main.c.o.requires:

.PHONY : CMakeFiles/MyClien.dir/main.c.o.requires

CMakeFiles/MyClien.dir/main.c.o.provides: CMakeFiles/MyClien.dir/main.c.o.requires
	$(MAKE) -f CMakeFiles/MyClien.dir/build.make CMakeFiles/MyClien.dir/main.c.o.provides.build
.PHONY : CMakeFiles/MyClien.dir/main.c.o.provides

CMakeFiles/MyClien.dir/main.c.o.provides.build: CMakeFiles/MyClien.dir/main.c.o


# Object files for target MyClien
MyClien_OBJECTS = \
"CMakeFiles/MyClien.dir/main.c.o"

# External object files for target MyClien
MyClien_EXTERNAL_OBJECTS =

MyClien.exe: CMakeFiles/MyClien.dir/main.c.o
MyClien.exe: CMakeFiles/MyClien.dir/build.make
MyClien.exe: CMakeFiles/MyClien.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/cygdrive/c/Users/Devon/CLionProjects/MyClien/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable MyClien.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MyClien.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MyClien.dir/build: MyClien.exe

.PHONY : CMakeFiles/MyClien.dir/build

CMakeFiles/MyClien.dir/requires: CMakeFiles/MyClien.dir/main.c.o.requires

.PHONY : CMakeFiles/MyClien.dir/requires

CMakeFiles/MyClien.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MyClien.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MyClien.dir/clean

CMakeFiles/MyClien.dir/depend:
	cd /cygdrive/c/Users/Devon/CLionProjects/MyClien/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /cygdrive/c/Users/Devon/CLionProjects/MyClien /cygdrive/c/Users/Devon/CLionProjects/MyClien /cygdrive/c/Users/Devon/CLionProjects/MyClien/cmake-build-debug /cygdrive/c/Users/Devon/CLionProjects/MyClien/cmake-build-debug /cygdrive/c/Users/Devon/CLionProjects/MyClien/cmake-build-debug/CMakeFiles/MyClien.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MyClien.dir/depend

