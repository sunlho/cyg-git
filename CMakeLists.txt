cmake_minimum_required(VERSION 3.30)
project(git C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} git.c)
