cmake_minimum_required(VERSION 3.17)
project(uart_debug)

set(CMAKE_CXX_STANDARD 20)

include_directories("../util")

add_executable(uart_debug main.cpp)