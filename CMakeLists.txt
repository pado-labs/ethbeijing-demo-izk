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
link_directories(${CMAKE_INSTALL_PREFIX}/lib)

add_executable(test_demo-izk test/demo-izk.cpp)
target_link_libraries(test_demo-izk ${EMP-OT_LIBRARIES} ${EMP-ZK_LIBRARIES})

add_executable(test_demo-http test/demo-http.cpp)
target_link_libraries(test_demo-http)

add_executable(test_demo-ecdsa test/demo-ecdsa.cpp)
target_link_libraries(test_demo-ecdsa secp256k1)

add_executable(test_demo test/demo.cpp)
target_link_libraries(test_demo ${EMP-OT_LIBRARIES} ${EMP-ZK_LIBRARIES} secp256k1)
