cmake_minimum_required(VERSION 3.10)
project(Projet_Algo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -g")

include_directories(include)

file(GLOB ALGO_SRC "src/Algo/*.cpp")
file(GLOB GRAPHE_SRC "src/Graphe/*.cpp")

set(SOURCES
    src/ChristoTotal.cpp 
    ${ALGO_SRC}
    ${GRAPHE_SRC}
)

add_executable(main ${SOURCES})
