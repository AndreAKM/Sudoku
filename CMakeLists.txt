cmake_minimum_required (VERSION 2.6)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project (Sudoku)

add_executable(Sudoku Sudoku.cpp Generator.cpp Parser.cpp Resolve.cpp)
