cmake_minimum_required(VERSION 3.23.2)

project(simple-vector)

add_executable(simple-vector simple-vector/main.cpp simple-vector/simple_vector.h simple-vector/array_ptr.h)

target_link_libraries(simple-vector)