cmake_minimum_required(VERSION 3.19)
project(GameOfLife C)
set(CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_C_STANDARD 11)
#list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake-sdl2)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(${PROJECT_NAME} main.c game.c game.h)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY})
#target_link_libraries(${PROJECT_NAME} SDL2::Main)