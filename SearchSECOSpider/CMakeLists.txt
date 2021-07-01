# This program has been developed by students from the bachelor Computer Science at
# Utrecht University within the Software Project course.
# © Copyright Utrecht University (Department of Information and Computing Sciences)

cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)

project(SearchSECOSpider)

add_subdirectory(../libSearchSECOSpider ./libSearchSECOSpider)

if (UNIX)
  list(APPEND CMAKE_CXX_FLAGS "-pthread ${CMAKE_CXX_FLAGS}")
endif(UNIX)

set(CMAKE_CXX_STANDARD 20)
set(EXE_NAME SearchSECOSpider)
add_executable(${EXE_NAME} main.cpp)
target_link_libraries(${EXE_NAME} libSearchSECOSpider)

target_include_directories(${EXE_NAME} PUBLIC "${PROJECT_BINARY_DIR}" ../libSearchSECOSpider)
