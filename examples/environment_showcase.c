/*******************************************************************************************
*
*   RayPals [Environment Showcase] - Example demonstrating environment sprites
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

    InitWindow(screenWidth, screenHeight, "RayPals - Environment Showcase Example");

    // Create our environment sprites
    RayPalsSprite* tree = CreateSimpleTree((Vector2){ 100, 300 }, 120, BROWN, GREEN);
    RayPalsSprite* cloud = CreateCloud((Vector2){ 250, 200 }, 80, WHITE);
    RayPalsSprite* bush = CreateBush((Vector2){ 400, 320 }, 80, DARKGREEN);
    
    // Create a custom rock sprite - smaller and gray
    RayPalsSprite* rock = CreateSprite(1);
    RayPals2DShape* rockShape = CreatePolygon((Vector2){ 0, 0 }, 30, 6, GRAY);
    AddShapeToSprite(rock, rockShape);
    SetSpritePosition(rock, (Vector2){ 550, 330 });
    
    RayPalsSprite* healthBar = CreateHealthBar((Vector2){ 670, 330 }, 150, 0.75f, RED, GREEN);
    
    // Add some simple animations
    float cloudX = 250.0f;
    float rockRotation = 0.0f;
    float treeScale = 1.0f;
    float treeScaleDir = 1.0f;
    float healthValue = 0.75f;
    float healthDir = -1.0f;
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        float deltaTime = GetFrameTime();
        
        // Move the cloud from left to right
        cloudX += 20.0f * deltaTime;
        if (cloudX > screenWidth + 50) cloudX = -50;
        SetSpritePosition(cloud, (Vector2){ cloudX, 200 });
        
        // Rotate the rock slowly
        rockRotation += 10.0f * deltaTime;
        SetSpriteRotation(rock, rockRotation);
        
        // Scale the tree up and down slightly
        treeScale += 0.2f * deltaTime * treeScaleDir;
        if (treeScale > 1.1f) treeScaleDir = -1.0f;
        if (treeScale < 0.9f) treeScaleDir = 1.0f;
        SetSpriteScale(tree, treeScale);
        
        // Update health bar
        healthValue += 0.3f * deltaTime * healthDir;
        if (healthValue > 1.0f) {
            healthValue = 1.0f;
            healthDir = -1.0f;
        } else if (healthValue < 0.0f) {
            healthValue = 0.0f;
            healthDir = 1.0f;
        }
        
        // Recreate the health bar with new value
        FreeSprite(healthBar);
        healthBar = CreateHealthBar((Vector2){ 670, 330 }, 150, healthValue, RED, GREEN);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(SKYBLUE);
            
            // Draw some ground
            DrawRectangle(0, 350, screenWidth, screenHeight - 350, DARKGREEN);
            
            // Draw our sprites in the right order (back to front)
            DrawSprite(cloud);
            DrawSprite(tree);
            DrawSprite(bush);
            DrawSprite(rock);
            DrawSprite(healthBar);
            
            // Draw labels
            DrawText("Tree", 90, 400, 20, WHITE);
            DrawText("Cloud", 230, 150, 20, DARKGRAY);
            DrawText("Bush", 390, 400, 20, WHITE);
            DrawText("Rock", 540, 400, 20, WHITE);
            DrawText("Health Bar", 640, 400, 20, WHITE);
            
            DrawFPS(screenWidth - 100, 20);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Free sprites
    FreeSprite(tree);
    FreeSprite(cloud);
    FreeSprite(bush);
    FreeSprite(rock);
    FreeSprite(healthBar);
    
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
} 