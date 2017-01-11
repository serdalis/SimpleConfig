# Custom build command script for use with CMake
#
# Delete the old CMake cache so we use the new configuration.
rm -rf "CMakeCache.txt"
#
# regenerate the build files using CMake
cmake -G "Unix Makefiles" .
#
# Build the project
# Need to use make since thats what the CI will be using.
# cmake --build . --target all --config Release
make