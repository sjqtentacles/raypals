/*******************************************************************************************
*
*   RayPals [Rotating Triangle] - Example showing how to create a rotating triangle
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

    InitWindow(screenWidth, screenHeight, "RayPals - Rotating Triangle Example");

    // Create a triangle
    RayPals2DShape* triangle = CreateTriangle((Vector2){ 400, 300 }, 100, RED);
    
    // Create another triangle with different color
    RayPals2DShape* triangle2 = CreateTriangle((Vector2){ 200, 200 }, 80, BLUE);
    
    // Create a third triangle with no fill
    RayPals2DShape* triangle3 = CreateTriangle((Vector2){ 600, 200 }, 80, GREEN);
    triangle3->filled = false;
    triangle3->thickness = 3.0f;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        float deltaTime = GetFrameTime();
        
        // Rotate triangles at different speeds
        RotateShape(triangle, deltaTime, 90.0f);   // 90 degrees per second
        RotateShape(triangle2, deltaTime, -120.0f); // -120 degrees per second
        RotateShape(triangle3, deltaTime, 60.0f);  // 60 degrees per second
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            // Draw the triangles
            Draw2DShape(triangle);
            Draw2DShape(triangle2);
            Draw2DShape(triangle3);
            
            // Draw instructions
            DrawText("Functional approach to rotating triangles", 20, 20, 20, DARKGRAY);
            DrawText("Each triangle rotates at a different speed", 20, 50, 20, DARKGRAY);
            DrawFPS(screenWidth - 100, 20);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Free our shapes
    FreeShape(triangle);
    FreeShape(triangle2);
    FreeShape(triangle3);
    
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
} 