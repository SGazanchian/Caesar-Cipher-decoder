cmake_minimum_required(VERSION 3.16)
project(OS C)

set(CMAKE_C_STANDARD 11)

add_executable(OS main.c)
add_executable(OS_Decoder decoder.c)
add_executable(OS_Finder finder.c)
add_executable(OS_Placer placer.c)