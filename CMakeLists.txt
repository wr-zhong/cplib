cmake_minimum_required(VERSION 3.20)

project(cp_lib LANGUAGES CXX)

add_library(cp_lib INTERFACE)

target_include_directories(cp_lib
    INTERFACE
        ${PROJECT_SOURCE_DIR}/include
)

# require c++20
target_compile_features(cp_lib INTERFACE cxx_std_20)


add_subdirectory(examples)