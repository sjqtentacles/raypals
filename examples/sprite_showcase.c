/*******************************************************************************************
*
*   RayPals [Sprite Showcase] - Example demonstrating various premade sprites
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

    InitWindow(screenWidth, screenHeight, "RayPals - Sprite Showcase Example");

    // Create our sprites
    RayPalsSprite* character = CreateSimpleCharacter((Vector2){ 100, 280 }, 80, BLUE, RED);
    RayPalsSprite* robot = CreateRobotCharacter((Vector2){ 250, 280 }, 80, DARKGRAY, GREEN);
    RayPalsSprite* animal = CreateAnimalCharacter((Vector2){ 400, 280 }, 80, ORANGE, BLACK);
    RayPalsSprite* ghost = CreateGhost((Vector2){ 550, 280 }, 80, PURPLE);
    RayPalsSprite* house = CreateHouse((Vector2){ 700, 280 }, 100, MAROON, DARKBROWN);
    
    // Create a simple animation for all sprites
    float characterRotation = 0.0f;
    float animalScale = 1.0f;
    float animalScaleDir = 1.0f;
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        float deltaTime = GetFrameTime();
        
        // Rotate the character
        characterRotation += 20.0f * deltaTime;
        if (characterRotation >= 360.0f) characterRotation -= 360.0f;
        SetSpriteRotation(character, characterRotation);
        
        // Rotate the robot the other way
        RotateSprite(robot, deltaTime, -25.0f);
        
        // Scale the animal up and down
        animalScale += 0.5f * deltaTime * animalScaleDir;
        if (animalScale > 1.2f) animalScaleDir = -1.0f;
        if (animalScale < 0.8f) animalScaleDir = 1.0f;
        SetSpriteScale(animal, animalScale);
        
        // Move the ghost up and down
        float ghostY = 280 + sinf(GetTime() * 2) * 20;
        SetSpritePosition(ghost, (Vector2){ 550, ghostY });
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            // Draw our sprites
            DrawSprite(character);
            DrawSprite(robot);
            DrawSprite(animal);
            DrawSprite(ghost);
            DrawSprite(house);
            
            // Draw labels
            DrawText("Simple", 75, 350, 20, DARKGRAY);
            DrawText("Character", 60, 370, 20, DARKGRAY);
            
            DrawText("Robot", 225, 350, 20, DARKGRAY);
            DrawText("Character", 210, 370, 20, DARKGRAY);
            
            DrawText("Animal", 375, 350, 20, DARKGRAY);
            DrawText("Character", 360, 370, 20, DARKGRAY);
            
            DrawText("Ghost", 530, 350, 20, DARKGRAY);
            
            DrawText("House", 680, 350, 20, DARKGRAY);
            
            DrawFPS(screenWidth - 100, 20);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Free sprites
    FreeSprite(character);
    FreeSprite(robot);
    FreeSprite(animal);
    FreeSprite(ghost);
    FreeSprite(house);
    
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
} 