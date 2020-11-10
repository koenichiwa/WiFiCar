cmake_minimum_required(VERSION 3.17)
project(util C)

set(CMAKE_C_STANDARD 99)

add_library(util SHARED vector.h drive_command.h)