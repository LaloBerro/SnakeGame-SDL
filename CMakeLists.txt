cmake_minimum_required(VERSION 3.25)
project(SnakeGame_SDL)

set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(SDL2_PATH "D:/Data/SharedLibrary/SDLMW/SDL2-2.28.5/x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(SnakeGame_SDL main.cpp)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
