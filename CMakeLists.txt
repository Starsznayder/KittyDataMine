project(kittyDataMine DESCRIPTION "KittyDataMine")
cmake_minimum_required(VERSION 3.10)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3 -msse3")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -ffast-math")


add_subdirectory(src)
#add_subdirectory(tests)
