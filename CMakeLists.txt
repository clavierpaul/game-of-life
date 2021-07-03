cmake_minimum_required(VERSION 3.19)
project(GameOfLife C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

find_package(SDL2 REQUIRED)
find_package(SDL2_TTF REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

find_package(BISON)
bison_target(RLEParser ${PROJECT_SOURCE_DIR}/parser/rle.y ${PROJECT_SOURCE_DIR}/parser/rle.tab.c)

find_package(FLEX)
flex_target(RLEScanner ${PROJECT_SOURCE_DIR}/parser/rle.l ${PROJECT_SOURCE_DIR}/parser/lex.yy.c)
ADD_FLEX_BISON_DEPENDENCY(RLEScanner RLEParser)

add_executable(${PROJECT_NAME} main.c game.c draw.c hashmap/hashmap.c ${BISON_RLEParser_OUTPUTS} ${FLEX_RLEScanner_OUTPUTS} parser/lex.yy.c parser/rle.c)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY})