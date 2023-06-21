cmake_minimum_required(VERSION 3.21)
project(TIPE_Simulation_de_Foules2 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(TIPE_Simulation_de_Foules2
        image.c
        image.h
        main.c
        map.c
        map.h
        min_heap.c
        min_heap.h
        move.c
        move.h
        person.c
        person.h)

target_link_libraries(${PROJECT_NAME} m)