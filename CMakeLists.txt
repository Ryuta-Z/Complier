project(test)
include_directories(include)
cmake_minimum_required(VERSION 3.5)
aux_source_directory(./src DIR_SRCS)
set(CMAKE_CXX_COMPILER "C:/Program Files (x86)/mingw-w64/bin/g++.exe")
add_executable(test ${DIR_SRCS})
