cmake_minimum_required(VERSION 3.25)
project(untitled CXX)

set(CMAKE_CXX_STANDARD 20)

# 1. Find all required SDL3 packages
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_mixer REQUIRED)

add_executable(untitled main.cpp GameEngine.cpp GameEngine.h)

# 2. Link all libraries to your executable
target_link_libraries(untitled PRIVATE
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf      # <--- Add this line
        SDL3_mixer::SDL3_mixer
)

# Copy Assets folder to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})