cmake_minimum_required(VERSION 3.22)
project(simple_tcp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost COMPONENTS system REQUIRED)

add_executable(tcp_server server/main.cpp)
target_link_libraries(tcp_server Boost::system)

add_executable(tcp_client client/main.cpp)
target_link_libraries(tcp_client Boost::system)