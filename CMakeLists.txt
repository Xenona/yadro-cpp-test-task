cmake_minimum_required(VERSION 3.10)

project (test_task VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

add_executable(test_task
main.cpp
)

target_include_directories(test_task PRIVATE .)

