cmake_minimum_required (VERSION 3.8)
project("CTP")

MESSAGE(STATUS "CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
MESSAGE(STATUS "CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../3rd/rapidjson/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include/)

set(ctp_collector ctpmkt.h ctpmkt.cpp ctptrd.h ctptrd.cpp test.cpp)
add_executable(${PROJECT_NAME} ${ctp_collector})


