cmake_minimum_required(VERSION 3.12)

project(StockMarketWebapp)

set(JWT_CPP_INCLUDE_DIR /home/omar/jwt-cpp/include)

find_package(OpenSSL REQUIRED)

set(EXECUTABLE_NAME main)

add_executable(${EXECUTABLE_NAME} main.cpp)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE jwt-cpp)

set_target_properties(${EXECUTABLE_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(jwt-cpp INTERFACE)
target_include_directories(jwt-cpp INTERFACE ${JWT_CPP_INCLUDE_DIR})
target_link_libraries(jwt-cpp INTERFACE OpenSSL::Crypto)