cmake_minimum_required(VERSION 3.0.0)
project(Memo2 VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output/)
include_directories(${PROJECT_SOURCE_DIR}/inc)
add_subdirectory(${PROJECT_SOURCE_DIR}/src)

add_executable(Memo2 main.cpp ${_SOURCES})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
