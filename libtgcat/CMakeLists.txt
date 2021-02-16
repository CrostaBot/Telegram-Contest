cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(tgcat)

add_library(tgcat SHARED src/tgcat.cpp)

set(CMAKE_CXX_FLAGS " -pthread -std=c++11")

#Tell CMake where the fastText CMakeLists.txt is located
add_subdirectory(resources/fastText)

#Here we add the fastText library to the tgcat target,
#you can also change static to shared if you want to build against the shared library
target_link_libraries(tgcat PRIVATE fasttext-static)
