################################################################################
#
#                              cleanup
#                     (c) 2015 Fabian Thüring
#
# This file is distributed under the MIT Open Source License. See 
# LICENSE.TXT for details.
#
################################################################################

cmake_minimum_required(VERSION  2.8.7)
project(cleanup)

if(NOT(CMAKE_MAJOR_VERSION LESS 3) AND NOT(CMAKE_MINOR_VERSION LESS 2))
    cmake_policy(SET CMP0054 OLD)
endif()

set(CLEANUP_VERSION_MAJOR "3")
set(CLEANUP_VERSION_MINOR "0")
set(CLEANUP_VERSION_STR "${CLEANUP_VERSION_MAJOR}.${CLEANUP_VERSION_MINOR}")

################################################################################
# Compiler Flags
################################################################################

# Use static runtime?
option(CLEANUP_LINK_STATIC 
       "Statically link against the C++/Boost runtime library" ${MSVC})
       
if(NOT(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC"))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -march=native -pthread")
else()
    if(CLEANUP_LINK_STATIC)
        # We will link statically against the C++ runtime library on Windows. 
        # The behaviour can be altered with ISEN_LINK_STATIC=OFF
        set(CompilerFlags
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
            CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
            CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
        foreach(CompilerFlag ${CompilerFlags})
            string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
            string(REPLACE "/MDd" "/MTd" ${CompilerFlag} "${${CompilerFlag}}")
        endforeach()
    else(CLEANUP_LINK_STATIC)
        # Instruct Boost to not perform auto-link
        add_definitions(-DBOOST_ALL_NO_LIB)
        add_definitions(-DBOOST_ALL_DYN_LINK)
    endif(CLEANUP_LINK_STATIC)
endif()

################################################################################
# CMake options
################################################################################
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
set(Boost_USE_STATIC_LIBS ${CLEANUP_LINK_STATIC}) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME ${CLEANUP_LINK_STATIC})

find_package(Boost COMPONENTS program_options filesystem system 
             serialization REQUIRED) 
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

################################################################################
# Find TBB
################################################################################
find_package(TBB REQUIRED)
include_directories(${TBB_INCLUDE_DIRS})
link_directories(${TBB_LIBRARY_DIRS})

################################################################################
# Compilation
################################################################################

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)

set(CLEANUP_SOURCE
    main.cpp
    CommandLine.cpp
    FileSystemBoost.cpp
    RemoveFile.cpp
    Reporter.cpp
    SearchPaths.cpp
)

set(CLEANUP_HEADER
    Config.h
    CommandLine.h
    Error.h
    FileSystemBoost.h
    RemoveFile.h
    Reporter.h
    SearchPaths.h
    Timer.h
    )

add_executable(cleanup ${CLEANUP_SOURCE} ${CLEANUP_HEADER})
target_link_libraries(cleanup ${Boost_LIBRARIES} ${TBB_LIBRARIES})

install(TARGETS cleanup RUNTIME DESTINATION bin)
