cmake_minimum_required(VERSION 3.16)
project(algoreppppp)

include(FetchContent)
FetchContent_Declare(spdlog
    URL https://github.com/gabime/spdlog/archive/v1.9.2.tar.gz
)
FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.7.3
)
FetchContent_MakeAvailable(spdlog)
FetchContent_GetProperties(json)
if(NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

add_definitions(-DOMPI_SKIP_MPICXX)
find_package(MPI REQUIRED)


add_custom_target(
    run.sh
    COMMAND sh ${CMAKE_SOURCE_DIR}/res/run.sh ${CMAKE_SOURCE_DIR}/res
    DEPENDS ${PROJECT_NAME}
)

add_executable(${PROJECT_NAME}
    src/main.cc
    src/runners/node.cc src/runners/node.hh
    src/runners/node/cycle/cycle.hh
    src/runners/node/cycle/followerCycle.hh
    src/runners/node/cycle/leaderCycle.hh
    src/runners/node/cycle/candidateCycle.hh
    src/runners/client/client.hh
    src/runners/client/client.cc

    src/wrappers/mpi_include.cc src/wrappers/mpi_include.hh
    src/wrappers/nlhomann_include.cc src/wrappers/nlhomann_include.hh
    src/wrappers/mpi/rpcRecieverReinjecter.cc src/wrappers/mpi/rpcRecieverReinjecter.hh
    src/wrappers/debug/add_stack_strace.cc src/wrappers/debug/add_stack_strace.hh
    src/wrappers/debug/print_log.hh

    src/rpc/rpc.cc src/rpc/rpc.hh
    src/rpc/message.cc src/rpc/message.hh
    src/rpc/requestVote.cc src/rpc/requestVote.hh
    src/rpc/appendEntries.cc src/rpc/appendEntries.hh
    src/rpc/rpcResponse.cc src/rpc/rpcResponse.hh
    src/rpc/requestLeader.cc src/rpc/requestLeader.hh

    src/config/globalConfig.cc src/config/globalConfig.hh
    src/runners/node/cycle/followerCycle.cc src/runners/node/cycle/candidateCycle.cc src/runners/node/cycle/leaderCycle.cc src/runners/node/cycle/cycle.cc src/runners/node/cycle/fwd.hh src/rpc/requestLeader.cc src/rpc/requestLeader.hh)

target_compile_features(${PROJECT_NAME} PRIVATE
    cxx_std_17
)
target_compile_options(${PROJECT_NAME} PRIVATE
    "-Wall"
    "-Wextra"
    $<$<CONFIG:Release>:-Werror -Ofast -flto -march=native>
    $<$<CONFIG:Debug>:-O0 -g3>
)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${MPI_CXX_INCLUDE_PATH}
)

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC
    ${MPI_CXX_LIBRARIES}
    spdlog::spdlog
    nlohmann_json::nlohmann_json
)
target_link_options(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Release>:>
    $<$<CONFIG:Debug>:-rdynamic -ldl>
)
