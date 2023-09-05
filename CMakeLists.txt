cmake_minimum_required(VERSION 3.26)
project(Cool6502)

set(CMAKE_CXX_STANDARD 20)


include_directories(external/lib6502-1.3)

set(SOURCE_FILES src/main.cpp external/lib6502-1.3/lib6502.c)

add_executable(Cool6502 ${SOURCE_FILES})


