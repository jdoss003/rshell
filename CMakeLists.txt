cmake_minimum_required(VERSION 3.6)
project(rshell)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS "-Wall -Werror -pedantic")

set(SOURCE_FILES src/RShell.cpp src/tasks/Task.cpp headers/tasks/Task.h src/Parser.cpp headers/Parser.h src/tasks/TaskList.cpp headers/tasks/TaskList.h src/tasks/ExitTask.cpp headers/tasks/ExitTask.h src/tasks/ExternalTask.cpp headers/tasks/ExternalTask.h src/tasks/ConditionalTask.cpp headers/tasks/ConditionalTask.h)
add_executable(rshell ${SOURCE_FILES})