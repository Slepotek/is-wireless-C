# Project name and cmake version
cmake_minimum_required(VERSION 3.18)
project(pathFinderC VERSION 1.0.0 LANGUAGES C)

# Project compile variables
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_SYSTEM_NAME Linux)

# GCC flags for Release and Debug build
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")

# If build type is not specified, build in Release mode
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Library source files (TODO: Update with each new lib module)
set(LIB_SOURCES
    body/matrixWorld.c
    body/path.c
    body/startingPointVector.c
    body/dfsPathFinding.c
    body/cli_handling.c)

# Library header files (TODO: Update with each new lib module)
set(LIB_HEADERS
    api-private/utilities.h
    api-private/matrixWorld.h
    api-private/path.h
    api-private/startingPointVector.h
    api-private/dfsPathFinding.h
    api-private/cli_handling.h)

# Library directories
set(LIB_DIRECTORIES
    api-private
    body)

# Create the pathFinderC_lib library
add_library(pathFinderC_lib STATIC ${LIB_SOURCES} ${LIB_HEADERS})
# Point to the library directories (to use only file names without dir)
target_include_directories(pathFinderC_lib PUBLIC ${LIB_DIRECTORIES})
# Set library properties
set_target_properties(pathFinderC_lib PROPERTIES
    C_STANDARD 23
    C_STANDARD_REQUIRED ON)

# Set the main function and project executable name
add_executable(pathFinderC main.c)
# Link libraries to the main
target_link_libraries(pathFinderC pathFinderC_lib)

# Option for running ctest suites
option(BUILD_TESTS "Build test suites" ON)
# TODO: add if statement when tests are enabled
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

install(TARGETS pathFinderC DESTINATION bin)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C standard: ${CMAKE_C_STANDARD}")
