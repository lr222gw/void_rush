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
        ${PROJECT_SOURCE_DIR}/deps/3Dproj/
        ${PROJECT_SOURCE_DIR}/deps/include/
        ${PROJECT_SOURCE_DIR}/src/*/
        ${PROJECT_SOURCE_DIR}/src/
        c:/Program Files (x86)/Windows Kits/10/Include/10.0.17134.0
)
link_directories(
        
        ${PROJECT_SOURCE_DIR}/deps/lib/
        ${PROJECT_SOURCE_DIR}/src/*/
)


FILE(GLOB_RECURSE src_SRCS src/*.cpp)
#FILE(GLOB_RECURSE src_SRCS_ */*.cpp)
FILE(GLOB_RECURSE src_HDRS src/*.hpp)
#FILE(GLOB_RECURSE src_HDRS_ */*.hpp)

File(GLOB_RECURSE renderer_src deps/3Dproj/*.cpp)
File(GLOB_RECURSE renderer_hdsr deps/3Dproj/*.h)
#${renderer_hdsr} ${renderer_src}

add_executable(vr WIN32 ${src_SRCS} ${src_SRCS_} ${src_HDRS} ${src_HDRS_}  ${renderer_src} ${renderer_hdsr} )
#${}

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

#target_link_libraries(${PROJECT_NAME} d3d11.lib)


target_include_directories(vr PUBLIC
        ${PROJECT_SOURCE_DIR}/src/*
        ${PROJECT_SOURCE_DIR}/src
        ${renderer_src}
)

target_link_libraries(${PROJECT_NAME} PRIVATE "d3d12.lib" "dxgi.lib" "d3dcompiler.lib")