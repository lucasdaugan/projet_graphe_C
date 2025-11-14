cmake_minimum_required(VERSION 3.16)
project(projet_graphe_C C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(projet_graphe
        main.c
        list.c
        adj_list.c
        utils.c
        hasse.c
