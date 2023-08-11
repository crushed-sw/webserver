cmake_minimum_required(VERSION 3.8)
project(webserver)

set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(include)
include_directories(example)

add_subdirectory(src)
add_subdirectory(example)
