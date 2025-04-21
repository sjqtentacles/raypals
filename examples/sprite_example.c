/*******************************************************************************************
*
*   RayPals [Sprite Example] - Example showing how to use sprites (shape compositions)
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

    InitWindow(screenWidth, screenHeight, "RayPals - Sprite Example");

    // Create some sprites
    RayPalsSprite* character = CreateSimpleCharacter((Vector2){ 200, 300 }, 60, RED, MAROON);
    RayPalsSprite* tree1 = CreateSimpleTree((Vector2){ 500, 400 }, 100, BROWN, GREEN);
    RayPalsSprite* tree2 = CreateSimpleTree((Vector2){ 600, 350 }, 80, BROWN, DARKGREEN);
    
    // Create a custom sprite (a simple house)
    RayPalsSprite* house = CreateSprite(3);
    
    // House base (square)
    RayPals2DShape* houseBase = CreateRectangle((Vector2){ 0, 10 }, (Vector2){ 100, 80 }, LIGHTGRAY);
    AddShapeToSprite(house, houseBase);
    
    // House roof (triangle)
    RayPals2DShape* houseRoof = CreateTriangle((Vector2){ 0, -40 }, 120, RED);
    AddShapeToSprite(house, houseRoof);
    
    // House door (rectangle)
    RayPals2DShape* houseDoor = CreateRectangle((Vector2){ 0, 30 }, (Vector2){ 20, 40 }, BROWN);
    AddShapeToSprite(house, houseDoor);
    
    // Set house position
    SetSpritePosition(house, (Vector2){ 400, 300 });

    // Create a custom cloud
    RayPalsSprite* cloud = CreateSprite(3);
    RayPals2DShape* cloud1 = CreateCircle((Vector2){ -20, 0 }, 30, WHITE);
    RayPals2DShape* cloud2 = CreateCircle((Vector2){ 0, -10 }, 40, WHITE);
    RayPals2DShape* cloud3 = CreateCircle((Vector2){ 20, 0 }, 30, WHITE);
    AddShapeToSprite(cloud, cloud1);
    AddShapeToSprite(cloud, cloud2);
    AddShapeToSprite(cloud, cloud3);
    SetSpritePosition(cloud, (Vector2){ 150, 100 });

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        float deltaTime = GetFrameTime();
        
        // Move character based on input
        if (IsKeyDown(KEY_RIGHT)) SetSpritePosition(character, (Vector2){ character->position.x + 100*deltaTime, character->position.y });
        if (IsKeyDown(KEY_LEFT)) SetSpritePosition(character, (Vector2){ character->position.x - 100*deltaTime, character->position.y });
        if (IsKeyDown(KEY_UP)) SetSpritePosition(character, (Vector2){ character->position.x, character->position.y - 100*deltaTime });
        if (IsKeyDown(KEY_DOWN)) SetSpritePosition(character, (Vector2){ character->position.x, character->position.y + 100*deltaTime });
        
        // Rotate some sprites
        RotateSprite(tree1, deltaTime, 15.0f);
        
        // Move cloud slowly
        SetSpritePosition(cloud, (Vector2){ cloud->position.x + 20*deltaTime, cloud->position.y });
        if (cloud->position.x > screenWidth + 100) SetSpritePosition(cloud, (Vector2){ -100, cloud->position.y });
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(SKYBLUE);
            
            // Draw background (ground)
            DrawRectangle(0, screenHeight - 100, screenWidth, 100, DARKGREEN);
            
            // Draw sprites
            DrawSprite(cloud);
            DrawSprite(house);
            DrawSprite(tree1);
            DrawSprite(tree2);
            DrawSprite(character);
            
            // Draw instructions
            DrawText("Use arrow keys to move the character", 20, 20, 20, WHITE);
            DrawFPS(screenWidth - 100, 20);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Free sprites
    FreeSprite(character);
    FreeSprite(tree1);
    FreeSprite(tree2);
    FreeSprite(house);
    FreeSprite(cloud);
    
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
} 