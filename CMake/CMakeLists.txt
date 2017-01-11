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
SET( CMAKE_CXX_FLAGS "-Wall -Werror -std=c++11" )

#
# Discover and store the source files for the main project.
#
file( GLOB_RECURSE main_files ${SRC_MAIN}/*.cpp ${SRC_MAIN}/*.h )

set( EXECUTABLE_OUTPUT_PATH ${OUT_EXE} )
set( LIBRARY_OUTPUT_PATH ${OUT_LIB} )

add_executable( SimpleConfig ${main_files} )
target_link_libraries ( SimpleConfig )

target_include_directories( SimpleConfig PUBLIC ${SRC_MAIN} )

install( TARGETS SimpleConfig DESTINATION build )