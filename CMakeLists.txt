# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.8)

project("6502")

# Include sub-projects.
add_subdirectory("Shared")
add_subdirectory("Assembler")
add_subdirectory("Emulator")