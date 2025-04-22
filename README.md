# RayPals

RayPals is a simple 2D/3D shape and sprite library built on top of raylib. It provides an easy-to-use interface for creating and manipulating shapes, sprites, and 3D objects for game development.

![RayPals Demo](examples/game_scene.gif)
![3D Sprites Example](examples/3d_sprites_example.gif)
![Basic Shapes](examples/basic_shapes.png)
![Sprite Gallery](examples/sprite_gallery.gif)

## Features

- **Basic 2D Shapes**
  - Square
  - Rectangle
  - Circle
  - Triangle
  - Star
  - Polygon
  - Arrow
  - Water Drop

- **Basic 3D Shapes**
  - Cube
  - Sphere
  - Cone
  - Cylinder

- **3D Models/Sprites**
  - Tree (trunk and foliage)
  - Robot (body, head, arms, and eyes)
  - Spaceship (body, wings, cockpit, engines)
  - More coming soon!

- **Sprite System**
  - Combine multiple shapes into sprites
  - Built-in sprite transformations (position, rotation, scale)
  - Animation support
  - Extensive collection of pre-built sprites

- **Pre-built Sprites**
  - Characters (Simple Character, Robot, Animal, Ghost, etc.)
  - Environment Objects (Tree, Cloud, House, Bush, Rock, etc.)
  - Vehicles (Car, Tank, Motorcycle, Airplane, UFO, etc.)
  - Items (Sword, Shield, Potion, Treasure Chest, etc.)
  - Magic Effects (Star, Lightning, Portal, Explosion, Waterfall)
  - UI Elements (Button, Health Bar)

- **Animation Support**
  - Scale animations
  - Rotation animations
  - Color transitions
  - Custom animation properties

## Installation

1. Make sure you have raylib installed on your system
2. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/raypals.git
   ```
3. Build using CMake:
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

## Usage Examples

### Creating a Simple 2D Shape

```c
#include "raypals.h"

// Create a red square
RayPals2DShape* square = CreateSquare(
    (Vector2){ 100, 100 },  // position
    50,                     // size
    RED                     // color
);

// Draw the shape
Draw2DShape(square);

// Don't forget to free when done
FreeShape(square);
```

### Creating a Water Drop Shape

```c
#include "raypals.h"

// Create a blue water drop
RayPals2DShape* waterDrop = CreateWaterDrop(
    (Vector2){ 200, 200 },  // position
    40,                     // size
    0.0f,                   // rotation
    SKYBLUE                 // color
);

// Draw the shape
Draw2DShape(waterDrop);

// Don't forget to free when done
FreeShape(waterDrop);
```

### Creating a Waterfall Sprite

```c
#include "raypals.h"

// Create a waterfall composed of multiple water drops
RayPalsSprite* waterfall = CreateWaterfallSprite(
    (Vector2){ 400, 300 },  // position
    200,                    // width
    300,                    // height
    SKYBLUE                 // color
);

// Draw the waterfall
DrawSprite(waterfall);

// Don't forget to free when done
FreeSprite(waterfall);
```

### Creating a Sprite

```c
#include "raypals.h"

// Create a character sprite
RayPalsSprite* character = CreateSimpleCharacter(
    (Vector2){ 400, 300 },  // position
    60,                     // size
    BLUE,                   // body color
    RED                     // head color
);

// Draw the sprite
DrawSprite(character);

// Free resources when done
FreeSprite(character);
```

### Creating and Manipulating 3D Models

```c
#include "raypals.h"

// Create a 3D tree
RayPals3DTree tree = Create3DTree(
    (Vector3){ 0.0f, 0.0f, 0.0f },  // position
    1.0f,                           // scale
    BROWN,                          // trunk color
    DARKGREEN                       // leaves color
);

// Create a 3D robot
RayPals3DSprite* robot = Create3DRobot(
    (Vector3){ 2.0f, 0.0f, 0.0f },  // position
    1.0f,                           // scale
    DARKGRAY,                       // body color
    GREEN                           // detail color
);

// Create a 3D spaceship
RayPals3DSprite* spaceship = Create3DSpaceship(
    (Vector3){ -2.0f, 2.0f, 0.0f }, // position
    1.0f,                           // scale
    LIGHTGRAY,                      // body color
    SKYBLUE                         // glass color
);

// All 3D models can be easily rotated and animated
Vector3 rotation = { 0.0f, animTime * 30.0f, 0.0f };  // Rotate around Y axis
Set3DSpriteRotation(tree.sprite, rotation);
Set3DSpriteRotation(robot, rotation);
Set3DSpriteRotation(spaceship, rotation);

// Draw all 3D models with the standard 3D sprite drawing function
Draw3DSprite(tree.sprite, camera);
Draw3DSprite(robot, camera);
Draw3DSprite(spaceship, camera);

// Free resources when done
Free3DTree(&tree);
Free3DSprite(robot);
Free3DSprite(spaceship);
```

- `game_scene.c`: Shows how to create a simple game scene
- `waterfall_example.c`: Shows how to create and animate a waterfall using water drop shapes

Run the examples from the build directory:
```bash
./examples/sprite_gallery
./examples/3d_sprites_example
./examples/3d_robot_example
./examples/waterfall_example
```