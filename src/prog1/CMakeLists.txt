cmake_minimum_required(VERSION 3.20)

project(prog1)
add_executable(prog1 prog1.cpp)

include_directories("../my_socket")

target_link_libraries(prog1 MySocket)
