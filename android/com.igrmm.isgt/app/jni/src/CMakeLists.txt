cmake_minimum_required(VERSION 3.6)

project(MY_APP)

find_library(SDL2 SDL2)
find_library(SDL2_image SDL2_image)

add_library(main SHARED)

target_sources(main PRIVATE ../../../../../src/main.c)

target_link_libraries(main SDL2main)
target_link_libraries(main SDL2)
target_link_libraries(main SDL2_image)


