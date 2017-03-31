cmake_minimum_required(VERSION 3.6)
project(Mi_TP)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c server.c client.c decoder.c decoder.h)
add_executable(Mi_TP ${SOURCE_FILES})