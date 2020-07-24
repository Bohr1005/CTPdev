cmake_minimum_required (VERSION 3.8)
project("CTP")

MESSAGE(STATUS "CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
MESSAGE(STATUS "CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../3rd/rapidjson/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include/)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/)
set(ctp_collector ctpmkt.h ctpmkt.cpp ctptrd.h ctptrd.cpp test.cpp)
add_executable(${PROJECT_NAME} ${ctp_collector})
target_link_libraries(${PROJECT_NAME} thostmduserapi_se.lib thosttraderapi_se)

