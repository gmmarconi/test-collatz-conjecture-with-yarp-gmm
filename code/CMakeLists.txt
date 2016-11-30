# Copyright: (C) 2010 RobotCub Consortium
# Author: Paul Fitzpatrick
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
set(PROJECTNAME collatz-conjecture)
project(${PROJECTNAME})
# YARP needs CMake 2.8.9 or greater
cmake_minimum_required(VERSION 2.8.9)
# find YARP
find_package(YARP REQUIRED)
# add YARP include directories
include_directories(${YARP_INCLUDE_DIRS})
# link with YARP libraries


cmake_minimum_required(VERSION 2.8.9)

#add_subdirectory(client)
add_subdirectory(server)
