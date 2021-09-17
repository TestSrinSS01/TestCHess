cmake_minimum_required(VERSION 3.18)
add_library(chess_engine chess.cpp)
target_include_directories(chess_engine PUBLIC ${CMAKE_SOURCE_DIR}/include)