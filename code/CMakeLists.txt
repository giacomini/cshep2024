cmake_minimum_required(VERSION 3.16)
project(cshep VERSION 0.1.0)

find_package(TBB REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra")

string(APPEND CMAKE_CXX_FLAGS_DEBUG
       " -fsanitize=address -fno-omit-frame-pointer")
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG
       " -fsanitize=address -fno-omit-frame-pointer")

add_executable(algo algo.cpp)
add_executable(algo_solution algo_solution.cpp)

add_executable(algo_functions algo_functions.cpp)
add_executable(algo_functions_solution algo_functions_solution.cpp)

add_executable(pack pack.cpp)
add_executable(sizeof sizeof.cpp)

#add_compile_definitions(EXTSIZE=256)
add_executable(cold cold.cpp)
add_executable(aos aos.cpp)
add_executable(aos_impr aos_impr.cpp)
add_executable(soa soa.cpp)

add_executable(race race.cpp)
add_executable(race_solution race_solution.cpp)

add_executable(false_sharing false_sharing.cpp)
add_executable(false_sharing_solution false_sharing_solution.cpp)

add_executable(privatization privatization.cpp)
add_executable(privatization_solution privatization_solution.cpp)

add_executable(parallel_for parallel_for.cpp)
target_link_libraries(parallel_for TBB::tbb)

add_executable(parallel_for_it parallel_for_it.cpp)
target_link_libraries(parallel_for_it TBB::tbb)

add_executable(parallel_reduce parallel_reduce.cpp)
target_link_libraries(parallel_reduce TBB::tbb)

add_executable(parallel_reduce_explicit parallel_reduce_explicit.cpp)
target_link_libraries(parallel_reduce_explicit TBB::tbb)
