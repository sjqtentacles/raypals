/*******************************************************************************************
*
*   Simple RayPals Example - Just copy this file to get started!
*
********************************************************************************************/

#include "raylib.h"
#include "raypals.h"  // Include the RayPals header

int main(void)
{
    // Initialize window
    InitWindow(800, 600, "Super Simple RayPals Example");
    SetTargetFPS(60);

    // Create some cool shapes
    RayPals2DShape* star = CreateStar((Vector2){400, 200}, 80, 5, YELLOW);
    RayPals2DShape* circle = CreateCircle((Vector2){200, 400}, 60, RED);
    
    // Create a character
    RayPalsSprite* character = CreateSimpleCharacter((Vector2){600, 400}, 100, BLUE, BEIGE);
    
    // Create a tree
    RayPalsSprite* tree = CreateSimpleTree((Vector2){100, 500}, 150, BROWN, GREEN);

    // Main game loop
    while (!WindowShouldClose())    
    {
        // Update
        float deltaTime = GetFrameTime();
        
        // Animate the star (rotate it)
        RotateShape(star, deltaTime, 45.0f);
        
        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            DrawText("RayPals Simple Example", 20, 20, 20, DARKGRAY);
            DrawText("Press ESC to exit", 20, 50, 15, GRAY);
            
            // Draw all our shapes and sprites
            Draw2DShape(star);
            Draw2DShape(circle);
            DrawSprite(character);
            DrawSprite(tree);
            
        EndDrawing();
    }

    // Cleanup
    FreeShape(star);
    FreeShape(circle);
    FreeSprite(character);
    FreeSprite(tree);
    
    CloseWindow();
    return 0;
} 