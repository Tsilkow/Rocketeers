cmake_minimum_required(VERSION 3.10)

project(Rocketeers VERSION 1.0)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories("inc")
file(GLOB SOURCES "src/*.cpp")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
add_library(Modules SHARED ${SOURCES})

add_executable(Rocketeers main.cpp)
target_compile_options(Rocketeers PRIVATE -Wall)
target_link_libraries(Rocketeers sfml-graphics sfml-audio Modules)
