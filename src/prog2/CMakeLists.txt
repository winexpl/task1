cmake_minimum_required(VERSION 3.20)

project(prog2)

add_executable(prog2 prog2.cpp)

include_directories("../my_socket")

target_link_libraries(prog2 MySocket)