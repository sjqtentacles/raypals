# RayPals

A simple 2D/3D shape and sprite library built on top of [raylib](https://www.raylib.com/). RayPals makes it easy to create and manipulate geometric shapes and sprites in your raylib projects.

![RayPals Demo](https://placeholder-for-screenshot.com/screenshot.png)

## What Does This Do?

RayPals gives you ready-made shapes and sprites for your raylib games:
- 🟥 Simple shapes: squares, circles, triangles
- 🎮 Game characters: simple humans, robots, animals 
- 🏠 Environment objects: trees, houses, clouds
- 🚗 Vehicles: cars, tanks, boats
- ⭐ Special effects: stars, lightning bolts
- Everything is already colored and animated!

## Super Easy Setup (No CMake Knowledge Required!)

### Method 1: Just Copy The Files! (Easiest)

1. Download this project (green "Code" button → "Download ZIP")
2. Extract the ZIP file
3. Copy these two files to your project:
   - `src/raypals.c`
   - `include/raypals.h`
4. Include in your code:
   ```c
   #include "raypals.h"
   ```
5. That's it! Use RayPals in your game!

### Method 2: Using Make (Pre-made script)

We've included a simple script to build everything for you:

```bash
# Copy and paste these commands:
git clone https://github.com/yourusername/raypals.git
cd raypals
./build_simple.sh  # Uses Make to build the library
```

## 5-Minute Getting Started Guide

```c
#include "raylib.h"
#include "raypals.h"  // Add this line to your code

int main() {
    // Open a window like usual
    InitWindow(800, 600, "My First RayPals Game");
    SetTargetFPS(60);

    // Create a cool star ⭐
    RayPals2DShape* star = CreateStar((Vector2){400, 300}, 100, 5, YELLOW);
    
    // Create a character 🧍
    RayPalsSprite* character = CreateSimpleCharacter((Vector2){200, 400}, 80, BLUE, RED);
    
    // Game loop
    while (!WindowShouldClose()) {
        // Rotate the star (easy animation!)
        RotateShape(star, GetFrameTime(), 45.0f);
        
        // Draw everything
        BeginDrawing();
            ClearBackground(RAYWHITE);
            Draw2DShape(star);
            DrawSprite(character);
            DrawText("My First RayPals Game!", 20, 20, 20, DARKGRAY);
        EndDrawing();
    }

    // Clean up
    FreeShape(star);
    FreeSprite(character);
    CloseWindow();
    return 0;
}
```

## Compiling Your Game

### Windows:
```
gcc your_game.c raypals.c -o your_game.exe -lraylib -lopengl32 -lgdi32 -lwinmm
```

### Mac:
```
gcc your_game.c raypals.c -o your_game -lraylib -framework OpenGL -framework Cocoa
```

### Linux:
```
gcc your_game.c raypals.c -o your_game -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

## Cool Objects You Can Create

### Shapes
```c
// Square (position, size, color)
RayPals2DShape* square = CreateSquare((Vector2){100, 100}, 50, RED);

// Circle (position, radius, color)
RayPals2DShape* circle = CreateCircle((Vector2){200, 200}, 40, BLUE);

// Triangle (position, size, color)
RayPals2DShape* triangle = CreateTriangle((Vector2){300, 300}, 60, GREEN);

// Star (position, size, points, color)
RayPals2DShape* star = CreateStar((Vector2){400, 400}, 70, 5, YELLOW);
```

### Ready-made Sprites
```c
// Character (position, size, body color, head color)
RayPalsSprite* hero = CreateSimpleCharacter((Vector2){400, 300}, 80, BLUE, BEIGE);

// Robot (position, size, body color, eyes color)
RayPalsSprite* robot = CreateRobotCharacter((Vector2){500, 300}, 70, GRAY, GREEN);

// Tree (position, size, trunk color, leaves color)
RayPalsSprite* tree = CreateSimpleTree((Vector2){100, 400}, 100, BROWN, DARKGREEN);

// Car (position, size, body color, wheels color)
RayPalsSprite* car = CreateCar((Vector2){300, 500}, 100, RED, BLACK);
```

### Easy Animation
```c
// Rotate a shape (shape, time since last frame, speed in degrees/second)
RotateShape(star, GetFrameTime(), 90.0f);

// Rotate a sprite
RotateSprite(car, GetFrameTime(), 45.0f);

// Change position
SetShapePosition(circle, (Vector2){mouseX, mouseY});  // Follow mouse!
```

## Still Want to Use CMake?

If you're familiar with CMake, you can still use our CMake setup:

```bash
# Clone the repo
git clone https://github.com/yourusername/raypals.git
cd raypals

# Build with CMake
mkdir build && cd build
cmake ..
cmake --build .

# Link with your project
# In your project's CMakeLists.txt:
# add_subdirectory(path/to/raypals)
# target_link_libraries(your_game raypals)
```

## Examples

Look in the `examples` folder for complete demos:
- `basic_shapes.c` - Creating simple shapes
- `sprite_gallery.c` - All sprites in one demo
- `game_scene.c` - A simple game scene with multiple objects

To run an example after building:
```bash
# From the build/examples directory:
./sprite_gallery
```

## Need Help?

- Check out the `include/raypals.h` file for all available functions
- Look at the examples in the `examples` folder 
- If something's not working, make sure raylib is installed correctly

## License

This is free software under the MIT License. Do whatever you want with it!

## Acknowledgments

- [raylib](https://www.raylib.com/) - The awesome library this is built on
