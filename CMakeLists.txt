cmake_minimum_required(VERSION 3.10)

project(stomplay VERSION 1.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(stomplay INTERFACE) 
target_include_directories(stomplay INTERFACE include/)
target_compile_definitions(stomplay INTERFACE STOMPLAY_VERSION=${PROJECT_VERSION})

