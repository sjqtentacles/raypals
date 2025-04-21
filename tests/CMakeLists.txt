cmake_minimum_required(VERSION 3.10)
project(raypals_tests)

# Find raylib package
find_package(raylib REQUIRED)

# Test executable
add_executable(raypals_tests 
    test_shapes.c
)

# Link against raylib and our library
target_link_libraries(raypals_tests PRIVATE raylib raypals)

# Include directories
target_include_directories(raypals_tests PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
)

# Add test
add_test(NAME raypals_tests COMMAND raypals_tests) 