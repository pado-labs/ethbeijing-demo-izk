cmake_minimum_required (VERSION 3.0)
project (demo-izk)
set(NAME "demo-izk")

find_path(CMAKE_FOLDER NAMES cmake/emp-tool-config.cmake)
include(${CMAKE_FOLDER}/cmake/emp-base.cmake)

find_package(emp-ot REQUIRED)
include_directories(${EMP-OT_INCLUDE_DIRS})
find_package(emp-zk REQUIRED)
include_directories(${EMP-ZK_INCLUDE_DIRS})


include_directories(.)
add_executable(test_demo-izk test/demo-izk.cpp)
target_link_libraries(test_demo-izk ${EMP-OT_LIBRARIES} ${EMP-ZK_LIBRARIES})
