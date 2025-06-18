cmake_minimum_required(VERSION 3.28.2)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(hachee)

include(cmake/CPM.cmake)
include(CTest)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/src/external/src/external/web-socket-networking/lib/networking/include)

add_subdirectory(src)
add_subdirectory(test)
