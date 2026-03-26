cmake_minimum_required(VERSION 3.16...4.1.1 FATAL_ERROR)
project(Argparser-Examples)

#--------------------------------------------------------------------
# Basic Sample Configures   
#--------------------------------------------------------------------

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(argparser_basic_cpp basic.cpp)
target_link_libraries(argparser_basic_cpp PRIVATE Argparser::Argparser)
