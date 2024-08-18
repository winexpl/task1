cmake_minimum_required (VERSION 3.20)
     
project(MySocket)
 
set(MY_SOCKET_SOURCE_FILES "./my_socket.cpp")
set(MY_SOCKET_HEADER_FILES "./my_socket.hpp")
 
add_library(MySocket STATIC ${MY_SOCKET_HEADER_FILES} ${MY_SOCKET_SOURCE_FILES})