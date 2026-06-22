cmake_minimum_required(VERSION 3.16)
project(stevensMathLib LANGUAGES CXX)

add_library(stevensMathLib INTERFACE)
add_library(stevensMathLib::stevensMathLib ALIAS stevensMathLib)

target_include_directories(stevensMathLib INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(stevensMathLib INTERFACE cxx_std_17)
