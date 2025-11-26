cmake_minimum_required(VERSION 3.26)
project(lab6_fla C)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab6_fla main.cpp
        analyzer.c)
set(CLANG_INCLUDE_DIR "C:/msys64/mingw64/lib/clang/21.1.5/include")
set(CLANG_LIB_DIR "C:/msys64/mingw64/bin")

target_include_directories(lab6_fla PUBLIC ${CLANG_INCLUDE_DIR})
target_link_directories(lab6_fla PUBLIC ${CLANG_LIB_DIR})
target_link_libraries(lab6_fla clang)