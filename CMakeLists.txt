cmake_minimum_required(VERSION 3.16)
project(projet_graphe_C C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Dossiers d’inclusion
include_directories(include)

add_executable(main
        src/graph/graph.c
        src/graph/cell.c
        src/graph/list.c
        cell.h
        list.c
        list.h
        adj_list.c
        adj_list.h
)
