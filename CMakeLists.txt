cmake_minimum_required(VERSION 3.25)
project(SnakeGame_SDL)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set(SDL2_DIR "D:/Data/SharedLibrary/SDLMW/SDL2-2.28.5/cmake")
set(SDL2_Image_DIR "D:/Data/SharedLibrary/SDLMW/SDL2_image-2.8.1/cmake")
find_package(SDL2 REQUIRED)
find_package(SDL2_Image REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} SDL2::SDL2main SDL2::SDL2 SDL2_image::SDL2_image)
set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE FALSE)