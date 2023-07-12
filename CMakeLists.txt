cmake_minimum_required(VERSION 3.8)
project(webserver)

set(PROJECT_NAME webserver)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

include_directories(include)

aux_source_directory(. SRC_LIST)
aux_source_directory(src SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST})
