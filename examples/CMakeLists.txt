cmake_minimum_required(VERSION 3.10)
project(raypals_examples C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find the raylib package
find_package(raylib REQUIRED)

# Define the examples to build
set(EXAMPLE_NAMES
    basic_shapes
    shape_animation
    shape_playground
    rotating_triangle
    sprite_example
    sprite_showcase
    sprite_gallery
    game_scene
    3d_sprites_example
)

# Create a target for each example
foreach(EXAMPLE_NAME ${EXAMPLE_NAMES})
    add_executable(${EXAMPLE_NAME} ${EXAMPLE_NAME}.c)
    
    # Link with raypals and raylib
    target_link_libraries(${EXAMPLE_NAME} PRIVATE raypals raylib)
    
    # Include directories
    target_include_directories(${EXAMPLE_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
    )
    
    # Install example
    install(TARGETS ${EXAMPLE_NAME} DESTINATION bin/examples)
endforeach() 