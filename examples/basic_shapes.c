/*******************************************************************************************
*
*   RayPals [Basic Shapes] - Simple example demonstrating 2D and 3D shapes
*
*   This example has been created using raylib 4.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2023 RayPals Team
*
********************************************************************************************/

#include "raylib.h"
#include "raypals.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "RayPals - Basic Shapes Example");

    // Define our camera for 3D shapes
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 4.0f, 4.0f, 4.0f };
    camera.target = (Vector3){ 0.0f, 0.5f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Create our 2D shapes
    RayPals2DShape* square = CreateSquare((Vector2){ 150, 150 }, 80, RED);
    RayPals2DShape* rectangle = CreateRectangle((Vector2){ 150, 300 }, (Vector2){ 120, 60 }, GREEN);
    RayPals2DShape* circle = CreateCircle((Vector2){ 300, 150 }, 40, BLUE);
    RayPals2DShape* triangle = CreateTriangle((Vector2){ 300, 300 }, 80, PURPLE);
    RayPals2DShape* star = CreateStar((Vector2){ 450, 150 }, 80, 5, YELLOW);
    
    // Set some properties
    square->filled = false;
    square->thickness = 3.0f;
    
    rectangle->rotation = 30.0f;
    
    triangle->filled = false;
    triangle->thickness = 2.0f;
    
    // Create our 3D shapes
    RayPals3DShape* cube = CreateCube((Vector3){ -1.0f, 0.5f, 0.0f }, (Vector3){ 1.0f, 1.0f, 1.0f }, RED);
    RayPals3DShape* sphere = CreateSphere((Vector3){ 1.0f, 0.5f, 0.0f }, 0.5f, 16, GREEN);
    RayPals3DShape* cone = CreateCone((Vector3){ 0.0f, 0.5f, -1.0f }, 0.5f, 1.0f, 16, BLUE);
    RayPals3DShape* cylinder = CreateCylinder((Vector3){ 0.0f, 0.5f, 1.0f }, 0.3f, 1.0f, 16, PURPLE);
    
    // Set some properties
    cube->wireframe = true;
    cube->thickness = 2.0f;
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // Update camera
        if (IsKeyDown(KEY_Z)) camera.position.y += 0.1f;
        if (IsKeyDown(KEY_X)) camera.position.y -= 0.1f;
        if (IsKeyDown(KEY_RIGHT)) camera.position.x += 0.1f;
        if (IsKeyDown(KEY_LEFT)) camera.position.x -= 0.1f;
        if (IsKeyDown(KEY_UP)) camera.position.z -= 0.1f;
        if (IsKeyDown(KEY_DOWN)) camera.position.z += 0.1f;
        
        // Update shape rotation
        cube->rotation.y += 1.0f;
        star->rotation += 0.5f;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            // Draw the 2D shapes
            Draw2DShape(square);
            Draw2DShape(rectangle);
            Draw2DShape(circle);
            Draw2DShape(triangle);
            Draw2DShape(star);
            
            // Draw 3D scene
            BeginMode3D(camera);
                
                // Draw grid for reference
                DrawGrid(10, 1.0f);
                
            EndMode3D();
            
            // Draw the 3D shapes
            Draw3DShape(cube, camera);
            Draw3DShape(sphere, camera);
            Draw3DShape(cone, camera);
            Draw3DShape(cylinder, camera);
            
            // Draw instructions
            DrawText("Use arrow keys and Z/X to move camera", 20, 20, 20, DARKGRAY);
            DrawFPS(screenWidth - 100, 20);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Free our shapes
    FreeShape(square);
    FreeShape(rectangle);
    FreeShape(circle);
    FreeShape(triangle);
    FreeShape(star);
    
    Free3DShape(cube);
    Free3DShape(sphere);
    Free3DShape(cone);
    Free3DShape(cylinder);
    
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
} 