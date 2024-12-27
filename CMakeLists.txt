cmake_minimum_required(VERSION 3.30)
project(Projekt C)

set(CMAKE_C_STANDARD 11)

add_library(Projekt STATIC library.c
        myfind.c
        myfind.h
        queue.h
        search.h
        queue.c
        search.c)
