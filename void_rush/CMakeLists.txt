cmake_minimum_required(VERSION 3.0.0)
project(vr VERSION 0.1.0)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -g ")

#include(CTest)
#enable_testing()
INCLUDE_DIRECTORIES(
        ${PROJECT_SOURCE_DIR}/src/*/
        ${PROJECT_SOURCE_DIR}/src/
)
link_directories(
        ${PROJECT_SOURCE_DIR}/src/*/
)

FILE(GLOB_RECURSE src_SRCS src/*.cpp)
#FILE(GLOB_RECURSE src_SRCS_ */*.cpp)
FILE(GLOB_RECURSE src_HDRS src/*.hpp)
#FILE(GLOB_RECURSE src_HDRS_ */*.hpp)

add_executable(vr ${src_SRCS} ${src_SRCS_} ${src_HDRS} ${src_HDRS_})
#${}

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_include_directories(vr PUBLIC
        ${PROJECT_SOURCE_DIR}/src/*
        ${PROJECT_SOURCE_DIR}/src
)