cmake_minimum_required(VERSION 3.0)
project(isgt)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(LIBS SDL2 SDL2_image)

foreach(LIB IN LISTS LIBS)
    find_package(${LIB} REQUIRED)
endforeach()

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} WIN32 src/main.c)

if(MINGW)
    target_link_libraries(${PROJECT_NAME} PRIVATE mingw32)
endif()

list(INSERT LIBS 0 SDL2main)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})
