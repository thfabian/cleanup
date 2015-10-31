################################################################################
#                           cleanup v2.0
#                     (c) 2015 Fabian Thüring
#
# This file is distributed under the MIT Open Source License. See 
# LICENSE.TXT for details.
#
################################################################################

set(CLEANUP_SOURCE
    cleanup.cpp
    fstask.cpp
    trash.cpp
)

set(CLEANUP_HEADER
    fstask.hpp
    trash.hpp
)

add_executable(cleanup ${CLEANUP_SOURCE} ${CLEANUP_HEADER})
target_link_libraries(cleanup ${Boost_LIBRARIES} ${TBB_LIBRARIES})

install(TARGETS cleanup RUNTIME DESTINATION bin)

