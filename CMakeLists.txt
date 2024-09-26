cmake_minimum_required(VERSION 3.29)
project(neo4j_test)

set(CMAKE_CXX_STANDARD 20)

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Download and make GoogleTest available
FetchContent_MakeAvailable(googletest)

enable_testing()

# Create the main executable
add_executable(neo4j_test
        GapFreeTracker.h
        GapFreeTrackerImpl.h
        GapFreeTrackerImpl.cpp
        main.cpp
)

# Create the test executable for Google Test
add_executable(neo4j_gtest
        GapFreeTrackerGTest.cpp
)

# Link the GoogleTest library to the test executable
target_link_libraries(neo4j_gtest gtest gtest_main)

# Link the main executable to necessary libraries if needed
# target_link_libraries(neo4j_test ...)

# Add the source files to the test executable
target_sources(neo4j_gtest PRIVATE
        GapFreeTracker.h
        GapFreeTrackerImpl.h
        GapFreeTrackerImpl.cpp
)

# Add Google Test discovery for the test executable
include(GoogleTest)
gtest_discover_tests(neo4j_gtest)
