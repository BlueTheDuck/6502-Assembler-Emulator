cmake_minimum_required(VERSION 3.8)

add_library(libassembler "opcodes.cpp"
                        "opcode_manager.cpp"
                        "assembler.cpp")


add_executable(assembler "main.cpp")

FILE(COPY opcodestable.tsv DESTINATION "${CMAKE_BINARY_DIR}/Assembler/")

target_link_libraries(assembler libassembler libshared libprimitives)