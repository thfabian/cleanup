################################################################################
#                           cleanup v2.0
#                     (c) 2015 Fabian Thüring
#
# This file is distributed under the MIT Open Source License. See 
# LICENSE.TXT for details.
#
################################################################################

cmake_minimum_required(VERSION  2.8.7)
project(cleanup)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -march=native -pthread")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
        "Choose the type of build, options are: Debug Release" FORCE)
    message(STATUS "Setting build type to 'Release' as none was specified.")
endif(NOT CMAKE_BUILD_TYPE)

# Inlcude all Find*.cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

################################################################################
# Find BOOST
################################################################################
find_package(Boost COMPONENTS program_options filesystem system REQUIRED) 
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    link_directories(${Boost_LIBRARY_DIRS})
endif()

################################################################################
# Find TBB
################################################################################
find_package(TBB REQUIRED)
if(TBB_FOUND)
    include_directories(${TBB_INCLUDE_DIRS})
    link_directories(${TBB_LIBRARY_DIRS})
endif()

################################################################################
# Compilation
################################################################################
add_subdirectory(src)

