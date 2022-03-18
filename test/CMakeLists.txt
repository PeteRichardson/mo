cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

add_executable( mo_test
    mo_test.cc
)

target_link_libraries( mo_test
    gtest_main
)

include(GoogleTest)
gtest_discover_tests( mo_test )