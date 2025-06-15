cmake_minimum_required(VERSION 3.30)
project(git C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CYGWIN_GIT_BIN "C:/cygwin64/bin/git.exe" CACHE STRING "Path to Cygwin Git binary")
set(CYGWIN_PREFIX "/cygdrive/" CACHE STRING "Cygwin mount prefix")
configure_file(config.h.in config.h)


add_executable(${PROJECT_NAME} git.c)

target_include_directories(git PRIVATE ${CMAKE_BINARY_DIR})
