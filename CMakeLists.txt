cmake_minimum_required(VERSION 3.13)
project(Boost_Echo_Client)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})
find_package(Boost COMPONENTS system filesystem REQUIRED)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}-std=c++11 -pthread -lboost_thread")

include_directories(bin)

include_directories(src)

add_executable(Boost_Echo_Client
        include/connectionHandler.h
        src/connectionHandler.cpp
        src/echoClient.cpp
        makefile include/clientToServerEncoder.h include/serverToClientDecoder.h src/clientToServerEncoder.cpp src/serverToClientDecoder.cpp)

target_link_libraries(Boost_Echo_Client ${Boost_LIBRARIES})
