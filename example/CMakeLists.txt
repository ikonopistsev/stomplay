cmake_minimum_required(VERSION 3.10)

project(e1 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("../../stomptalk/include")

add_executable(e1 e1.cpp)

target_link_libraries(e1 PRIVATE stomplay)
