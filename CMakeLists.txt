cmake_minimum_required(VERSION 3.10)
file(GLOB src "src/*.c")

project(c-wordle)
add_executable(c-wordle ${src})