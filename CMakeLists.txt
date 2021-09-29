cmake_minimum_required(VERSION 3.20)
project(sudoku)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -pedantic -Werror)

add_executable(sudoku main.cpp Row.cpp Row.h Sudoku.cpp Sudoku.h NumberVector.cpp NumberVector.h RowArray.cpp RowArray.h NumberVectorStaticArrays.h)
