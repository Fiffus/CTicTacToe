cmake_minimum_required(VERSION 3.28)
project(TicTacToe C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2TTF REQUIRED SDL2_ttf)

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2TTF_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

file(COPY ${CMAKE_SOURCE_DIR}/res/font DESTINATION ${CMAKE_BINARY_DIR}/res)

link_directories(
    ${SDL2_LIBRARY_DIRS}
    ${SDL2TTF_LIBRARY_DIRS}
)

file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.c"
)

file(GLOB_RECURSE HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/**/*.h"
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARIES}
    ${SDL2TTF_LIBRARIES}
)
