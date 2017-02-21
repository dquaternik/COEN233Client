cmake_minimum_required(VERSION 3.6)
project(MyClien)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c)
add_executable(MyClien ${SOURCE_FILES})