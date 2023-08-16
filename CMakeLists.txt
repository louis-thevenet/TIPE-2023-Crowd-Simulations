cmake_minimum_required(VERSION 3.23)
project(crowdsim
    VERSION 1.0
    DESCRIPTION "A crowd simulation"
    LANGUAGES C)

    add_executable(crowdsim
    src/image.c
    src/map.c
    src/move.c
    src/min_heap.c
    src/person.c
    src/main.c
    )
target_include_directories(crowdsim PRIVATE ${CMAKE_CURRENT_LIST_DIR}/headers)
target_link_libraries(crowdsim PRIVATE m)
    target_compile_features(crowdsim PRIVATE c_std_23)
