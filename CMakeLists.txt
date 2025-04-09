cmake_minimum_required(VERSION 3.10)
project(Flappy)

set(CMAKE_C_STANDARD 11)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_image_INCLUDE_DIRS})

set(SOURCES 
    src/main.c
)

add_executable(Flappy ${SOURCES})

target_link_libraries(Flappy SDL2::SDL2 SDL2_image::SDL2_image)
