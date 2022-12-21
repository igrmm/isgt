project(hello)
cmake_minimum_required(VERSION 3.0)

add_executable(hello src/main.c)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

target_link_libraries(hello PRIVATE SDL2)
target_link_libraries(hello PRIVATE SDL2_image)
