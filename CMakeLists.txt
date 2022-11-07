cmake_minimum_required(VERSION 3.23)
project(nsmfsqe)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenMP REQUIRED)
include_directories(${PROJECT_SOURCE_DIR}/excerpt/include)
add_executable(${CMAKE_PROJECT_NAME} main.cpp Timer.cpp Timer.h)
target_link_libraries(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCE_DIR}/excerpt/libexcerpt.dll OpenMP::OpenMP_CXX)
file(COPY ${PROJECT_SOURCE_DIR}/excerpt/libexcerpt.dll DESTINATION ${PROJECT_BINARY_DIR})
