cmake_minimum_required(VERSION 3.14)
project(IMU CXX C ASM)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(googletest)

add_library(mathmart INTERFACE)

target_include_directories(mathmart
    INTERFACE
    Inc
    )

add_executable(testMathmart
    tests/testKalman.cpp
    )

target_link_libraries(testMathmart
    mathmart
    gtest
    gtest_main
    )
