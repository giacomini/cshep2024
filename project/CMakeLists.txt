cmake_minimum_required(VERSION 3.16)
project(mandelbrot VERSION 0.1.0)

find_package(TBB REQUIRED)
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra")

string(APPEND CMAKE_CXX_FLAGS_DEBUG
       " -fsanitize=address -fno-omit-frame-pointer")
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG
       " -fsanitize=address -fno-omit-frame-pointer")

add_executable(mandelbrot main.cpp)
target_link_libraries(mandelbrot PRIVATE sfml-graphics)

#add_executable(mandelbrot_par main_par.cpp)
#target_link_libraries(mandelbrot_par PRIVATE TBB::tbb sfml-graphics)
