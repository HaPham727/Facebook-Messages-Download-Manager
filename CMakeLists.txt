cmake_minimum_required(VERSION 3.10.0)
project(Facebook-Messages-Download-Manager VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 26)

add_executable(Facebook-Messages-Download-Manager main.cpp)

include(CTest)
enable_testing()

