cmake_minimum_required(VERSION 3.10)

project(stomplay VERSION 1.0.0 LANGUAGES CXX)
add_definitions("-DSTOMPLAY_VERSION=${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# temporary
include_directories("../stomptalk/include")

add_library(stomplay INTERFACE) 

target_include_directories(stomplay INTERFACE include/)

option(STOMPLAY_BUILD_EXAMPLES "build examples" OFF)

if (STOMPLAY_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()
