cmake_minimum_required(VERSION 3.16)

project(wasm-mouse-entropy)

#include_directories(config)
#include(cmake/wolfssl.cmake)

if (NOT EMSCRIPTEN)
    message(FATAL_ERROR "This project needs to be built with Emscripten.")
endif()

add_executable(entropy-gen
    src/main.c)

target_link_options(entropy-gen PRIVATE -sEXPORTED_RUNTIME_METHODS=["cwrap","ccall"])
set(CMAKE_EXECUTABLE_SUFFIX ".html")
    

#target_link_libraries(entropy-gen PRIVATE wolfssl)
target_include_directories(entropy-gen PRIVATE inc)


