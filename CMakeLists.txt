cmake_minimum_required(VERSION 3.23)
project(nsmfsqe)

set(CMAKE_CXX_STANDARD 14)

include_directories(${PROJECT_SOURCE_DIR}/excerpt/include)
#add_subdirectory(${PROJECT_SOURCE_DIR}/excerpt)

add_executable(nsmfsqe main.cpp)

target_link_libraries(nsmfsqe ${PROJECT_SOURCE_DIR}/excerpt/libexcerpt.dll)
file(COPY ${PROJECT_SOURCE_DIR}/excerpt/libexcerpt.dll
        DESTINATION ${PROJECT_BINARY_DIR})
