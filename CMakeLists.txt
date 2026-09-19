cmake_minimum_required(VERSION 3.28)
project(tetris_sdl CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew" "/usr/local")

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)

include_directories(include)

add_executable(${PROJECT_NAME}
        src/main.cpp
        include/Game.h
        src/Game.cpp
        include/Grid.h
        src/Grid.cpp
        include/Colors.h
        src/Colors.cpp
        src/Colors.cpp
        include/Block.h
        src/Block.cpp
        src/Blocks.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        SDL3::SDL3
        SDL3_image::SDL3_image
)