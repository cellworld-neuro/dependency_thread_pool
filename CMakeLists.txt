cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)

project(thread_pool
        VERSION 2022.1.0
        DESCRIPTION "Simple parallel workers framework"
        LANGUAGES CXX)

include_directories(include)

string(APPEND CMAKE_CXX_FLAGS " -fno-strict-aliasing -pthread ")

###
### TESTS
###

add_executable(thread_pool_tests tests/thread_pool.cpp)

add_executable(member tests/member.cpp)


###
### LIBRARY INSTALLATION
###

include(GNUInstallDirs)

install(FILES include/thread_pool.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
