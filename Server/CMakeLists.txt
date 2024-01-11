cmake_minimum_required(VERSION 3.12)

project(StockMarketWebapp)

set(JWT_CPP_INCLUDE_DIR /home/omar/jwt-cpp/include)


find_package(OpenSSL REQUIRED)

add_library(jwt-cpp INTERFACE)
target_include_directories(jwt-cpp INTERFACE ${JWT_CPP_INCLUDE_DIR})
target_link_libraries(jwt-cpp INTERFACE OpenSSL::Crypto)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE jwt-cpp)
