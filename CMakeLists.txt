cmake_minimum_required(VERSION 3.16)
project(algoreppppp)

find_package(MPI REQUIRED)
add_definitions(-DOMPI_SKIP_MPICXX)


add_executable(${PROJECT_NAME}
    main.cc
    src/client.cc include/client.hh
    src/server.cc include/server.hh
)
target_compile_features(${PROJECT_NAME} PRIVATE
    cxx_std_17
)
target_compile_options(${PROJECT_NAME} PRIVATE
#    "-Wall"
#    "-Wextra"
    $<$<CONFIG:Release>:-Werror -Ofast -flto -march=native>
    $<$<CONFIG:Debug>:-O0 -ggdb>
#    "-pedantic"
)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    include/
    ${MPI_C_INCLUDE_PATH}
)

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC
    ${MPI_C_LIBRARIES}
        -pthread
)