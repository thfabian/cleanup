################################################################################
#    cleanup - v2.0	
#    (c) 2015 Fabian Thüring
#
# Scans all subdirectories, starting at $HOME, for files matching a specific 
# pattern (e.g linux backup files [*~] ) and moves them to trash.
#
################################################################################

cmake_minimum_required(VERSION 2.8.12)
project(cleanup)

set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG -Wall")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Choose the type of build, options are: Debug Release" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

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
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake/FindTBB/")
find_package(TBB REQUIRED)
if(TBB_FOUND)
    include_directories(${TBB_INCLUDE_DIRS})
    link_directories(${TBB_LIBRARY_DIRS})
endif()

################################################################################
# Compilation
################################################################################
file(GLOB sources "*.cpp")
add_executable(cleanup ${sources})
target_link_libraries(cleanup ${Boost_LIBRARIES} ${TBB_LIBRARIES})

################################################################################
# Installation
################################################################################
install(TARGETS cleanup RUNTIME DESTINATION bin)

################################################################################
# Uninstallation
################################################################################
add_custom_target(uninstall 
      "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_SOURCE_DIR}/CMake/unistall.cmake")
