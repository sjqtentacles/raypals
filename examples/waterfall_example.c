/*******************************************************************************************
*
*   RayPals [Waterfall Example] - Example demonstrating the waterfall sprite
*
*   This example has been created using raylib 4.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2023 RayPals Team
*
********************************************************************************************/

#include "raylib.h"
#include "raypals.h"
#include <math.h>  // For sinf function

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "RayPals - Waterfall Example");
    
    // Create waterfall sprite
    RayPalsSprite* waterfall = CreateWaterfallSprite(
        (Vector2){ screenWidth/2, screenHeight/2 },  // Center of the screen
        200,                                          // Width
        300,                                          // Height
        (Color){ 100, 150, 255, 200 }                // Light blue with transparency
    );
    
    // Create some environment elements
    RayPalsSprite* leftRock = CreateRock(
        (Vector2){ screenWidth/2 - 150, screenHeight/2 + 50 },
        80,
        DARKGRAY
    );
    
    RayPalsSprite* rightRock = CreateRock(
        (Vector2){ screenWidth/2 + 150, screenHeight/2 + 50 },
        90,
        GRAY
    );
    
    // Pool of water at the bottom
    RayPals2DShape* pool = CreateCircle(
        (Vector2){ screenWidth/2, screenHeight/2 + 170 },
        120,
        (Color){ 100, 150, 255, 180 }
    );
    
    // Background elements
    RayPalsSprite* tree1 = CreateSimpleTree(
        (Vector2){ screenWidth/2 - 250, screenHeight/2 + 100 },
        100,
        BROWN,
        DARKGREEN
    );
    
    RayPalsSprite* tree2 = CreateSimpleTree(
        (Vector2){ screenWidth/2 + 230, screenHeight/2 + 80 },
        120,
        BROWN,
        DARKGREEN
    );
    
    RayPalsSprite* cloud = CreateCloud(
        (Vector2){ screenWidth/2 - 150, screenHeight/2 - 200 },
        80,
        WHITE
    );
    
    // Animation variables
    float animTime = 0.0f;
    const Color skyColor = (Color){ 200, 230, 255, 255 };
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        float deltaTime = GetFrameTime();
        animTime += deltaTime;
        
        // Animate waterfall by offsetting water drops
        Vector2 offset = { 
            sinf(animTime * 2.0f) * 2.0f,  // Gentle side-to-side motion
            sinf(animTime * 5.0f) * 1.0f   // Subtle vertical pulsing
        };
        SetSpritePosition(waterfall, (Vector2){ screenWidth/2 + offset.x, screenHeight/2 + offset.y });
        
        // Animate cloud
        Vector2 cloudPos = { 
            screenWidth/2 - 150 + sinf(animTime * 0.3f) * 50.0f,  // Slow horizontal movement
            screenHeight/2 - 200 
        };
        SetSpritePosition(cloud, cloudPos);
        
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        
        ClearBackground(skyColor);
        
        // Draw mountains in background
        DrawTriangle(
            (Vector2){ screenWidth/2 - 300, screenHeight/2 + 100 },
            (Vector2){ screenWidth/2 - 50, screenHeight/2 - 150 },
            (Vector2){ screenWidth/2 + 200, screenHeight/2 + 100 },
            (Color){ 120, 120, 120, 255 }
        );
        
        DrawTriangle(
            (Vector2){ screenWidth/2 - 100, screenHeight/2 + 100 },
            (Vector2){ screenWidth/2 + 150, screenHeight/2 - 180 },
            (Vector2){ screenWidth/2 + 350, screenHeight/2 + 100 },
            (Color){ 100, 100, 100, 255 }
        );
        
        // Draw ground
        DrawRectangle(0, screenHeight/2 + 100, screenWidth, screenHeight/2, (Color){ 100, 180, 100, 255 });
        
        // Draw scene elements
        DrawSprite(tree1);
        DrawSprite(tree2);
        DrawSprite(cloud);
        
        // Draw water pool
        Draw2DShape(pool);
        
        // Draw waterfall
        DrawSprite(waterfall);
        
        // Draw rocks that frame the waterfall
        DrawSprite(leftRock);
        DrawSprite(rightRock);
        
        // Draw instructions and FPS
        DrawText("ESC to exit", 20, 20, 20, DARKGRAY);
        DrawFPS(screenWidth - 100, 20);
        
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    
    // Free resources
    FreeSprite(waterfall);
    FreeSprite(leftRock);
    FreeSprite(rightRock);
    FreeShape(pool);
    FreeSprite(tree1);
    FreeSprite(tree2);
    FreeSprite(cloud);
    
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
} 