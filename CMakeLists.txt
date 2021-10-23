cmake_minimum_required(VERSION 3.16)
project(algoreppppp)

include(FetchContent)
FetchContent_Declare(spdlog
    URL https://github.com/gabime/spdlog/archive/v1.9.2.tar.gz
)
FetchContent_MakeAvailable(spdlog)

add_definitions(-DOMPI_SKIP_MPICXX)
find_package(MPI REQUIRED)


add_custom_target(
    run.sh
    COMMAND sh ${CMAKE_SOURCE_DIR}/run.sh
    DEPENDS ${PROJECT_NAME}
)

add_executable(${PROJECT_NAME}
    main.cc
    src/runners/node.cc src/runners/node.hh
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
    ${MPI_CXX_INCLUDE_PATH}
)

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC
    ${MPI_CXX_LIBRARIES}
    spdlog::spdlog
)
