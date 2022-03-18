cmake_minimum_required(VERSION 3.18)

project(mo)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)
FetchContent_Declare(
    cpp-mmf
    GIT_REPOSITORY https://github.com/carlomilanesi/cpp-mmf.git
    GIT_TAG         v0.0.1
)
FetchContent_MakeAvailable(cpp-mmf)

add_executable(mo
    src/mo.cc
    src/macho.cc
    ${cpp-mmf_SOURCE_DIR}/memory_mapped_file.cpp)

include_directories(${cpp-mmf_SOURCE_DIR})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/templ ${CMAKE_BINARY_DIR}/templ COPYONLY)


enable_testing()
add_subdirectory(test)


