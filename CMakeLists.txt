cmake_minimum_required( VERSION 2.8 )

project( SimpleConfig )

#
# Store all our paths in variables.
#
set( SRC_MAIN "../Linux_G++" )
set( OUT_EXE "../Linux_G++/bin" )
set( OUT_LIB "../Linux_G++/lib" )

#
# Add compilation flags.
#
set(DEBUG_FLAGS "-std=c++11 -g -O1 -Wall -Wextra -Werror -pedantic")
set(RELEASE_FLAGS "-std=c++11 -O3 -Wall -Wextra -Werror -pedantic")

set(CMAKE_CXX_FLAGS ${RELEASE_FLAGS})
set(CMAKE_CXX_FLAGS_DEBUG ${DEBUG_FLAGS})
set(CMAKE_CONFIGURATION_TYPES Debug Release)


#
# Discover and store the source files for the main project.
#
file( GLOB_RECURSE main_files ${SRC_MAIN}/*.cpp ${SRC_MAIN}/*.h )

set( EXECUTABLE_OUTPUT_PATH ${OUT_EXE} )
set( LIBRARY_OUTPUT_PATH ${OUT_LIB} )

include_directories( ${SRC_MAIN} )