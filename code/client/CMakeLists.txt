# Copyright: (C) 2010 RobotCub Consortium
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
project(collatz-client)
# YARP needs CMake 2.8.9 or greater
cmake_minimum_required(VERSION 2.8.9)
# find YARP
find_package(YARP REQUIRED)
# add YARP include directories
include_directories(${YARP_INCLUDE_DIRS})

set(folder_header ${folder_header}    collatzClient.h)
set(folder_source ${folder_source}    collatzClient.cpp
                                      cClient_main.cpp
                                      )

source_group("Source Files" FILES ${folder_source})
source_group("Header Files" FILES ${folder_header})
# set up our program
add_executable(${PROJECT_NAME} ${folder_source} ${folder_header})
# link with YARP libraries
target_link_libraries(${PROJECT_NAME} ${YARP_LIBRARIES})
