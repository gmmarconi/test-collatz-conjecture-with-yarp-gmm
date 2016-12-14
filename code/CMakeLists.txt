# Copyright: (C) 2010 RobotCub Consortium
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
cmake_minimum_required(VERSION 2.8.9)
# YARP needs CMake 2.8.9 or greater
project(collatz-conjecture)
# find YARP
find_package(YARP REQUIRED)
# add YARP include directories
include_directories(${YARP_INCLUDE_DIRS})
add_subdirectory(client)
add_subdirectory(server)
