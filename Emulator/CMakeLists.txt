cmake_minimum_required(VERSION 3.8)

add_library(libemulator "processor_state.cpp")

add_executable(emulator "main.cpp")

target_link_libraries(emulator libemulator libshared libprimitives)