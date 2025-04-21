/*******************************************************************************************
*
*   RayPals [Sprite Gallery] - Example demonstrating all available sprite types
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
#include <string.h> // For strcmp function
#include <stdio.h>  // For printf function

// Structure to store sprite information
typedef struct {
    RayPalsSprite* sprite;
    const char* name;
    const char* category;
    bool animated;
} SpriteInfo;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "RayPals - Sprite Gallery");
    
    // Define sprite grid properties
    const int gridCols = 4;       // Number of columns in the grid
    const int cellWidth = 180;    // Width of each grid cell
    const int cellHeight = 200;   // Height of each grid cell
    const int cellPadding = 20;   // Padding between cells
    
    float scrollY = 0.0f;         // Scroll position
    const float scrollSpeed = 300.0f;

    // Create all available sprites
    SpriteInfo sprites[] = {
        // Characters
        { CreateSimpleCharacter((Vector2){0, 0}, 60, BLUE, RED), "Simple Character", "Characters", true },
        { CreateRobotCharacter((Vector2){0, 0}, 60, DARKGRAY, GREEN), "Robot", "Characters", true },
        { CreateAnimalCharacter((Vector2){0, 0}, 60, ORANGE, BLACK), "Animal", "Characters", true },
        { CreateGhost((Vector2){0, 0}, 60, PURPLE), "Ghost", "Characters", true },
        { CreateSoldier((Vector2){0, 0}, 60, GREEN, BEIGE), "Soldier", "Characters", true },
        { CreateZombie((Vector2){0, 0}, 60, BEIGE, DARKGREEN), "Zombie", "Characters", true },
        { CreateWizard((Vector2){0, 0}, 60, PURPLE, BLUE), "Wizard", "Characters", true },
        { CreateDragon((Vector2){0, 0}, 60, RED, ORANGE), "Dragon", "Characters", true },
        
        // Environment
        { CreateSimpleTree((Vector2){0, 0}, 80, BROWN, GREEN), "Tree", "Environment", false },
        { CreateCloud((Vector2){0, 0}, 60, WHITE), "Cloud", "Environment", false },
        { CreateHouse((Vector2){0, 0}, 80, MAROON, DARKBROWN), "House", "Environment", false },
        { CreateBush((Vector2){0, 0}, 60, DARKGREEN), "Bush", "Environment", false },
        { CreateRock((Vector2){0, 0}, 70, GRAY), "Rock", "Environment", false },
        { CreateFlower((Vector2){0, 0}, 60, PINK, YELLOW), "Flower", "Environment", false },
        { CreateFish((Vector2){0, 0}, 70, SKYBLUE, BLUE), "Fish", "Environment", false },
        { CreateSnowman((Vector2){0, 0}, 70, WHITE, BLACK), "Snowman", "Environment", true },
        
        // Vehicles
        { CreateCar((Vector2){0, 0}, 70, RED, BLACK), "Car", "Vehicles", false },
        { CreateTank((Vector2){0, 0}, 70, GREEN, DARKGRAY), "Tank", "Vehicles", false },
        { CreateMotorcycle((Vector2){0, 0}, 70, BLUE, BLACK), "Motorcycle", "Vehicles", false },
        { CreateSkateboard((Vector2){0, 0}, 70, YELLOW, BLACK), "Skateboard", "Vehicles", false },
        { CreateSailboat((Vector2){0, 0}, 70, BROWN, WHITE), "Sailboat", "Vehicles", false },
        { CreateAirplane((Vector2){0, 0}, 70, SKYBLUE, WHITE), "Airplane", "Vehicles", true },
        { CreateUFO((Vector2){0, 0}, 70, GRAY, GREEN), "UFO", "Vehicles", true },
        
        // Items
        { CreateSword((Vector2){0, 0}, 70, LIGHTGRAY, BROWN), "Sword", "Items", false },
        { CreateArrowSprite((Vector2){0, 0}, 70, BROWN, RED), "Arrow", "Items", false },
        { CreateKey((Vector2){0, 0}, 70, GOLD), "Key", "Items", false },
        { CreateShield((Vector2){0, 0}, 70, BROWN, GOLD), "Shield", "Items", false },
        { CreateCrown((Vector2){0, 0}, 70, GOLD, RED), "Crown", "Items", false },
        { CreateCoin((Vector2){0, 0}, 70, GOLD), "Coin", "Items", false },
        { CreateGem((Vector2){0, 0}, 70, PURPLE), "Gem", "Items", false },
        { CreatePotion((Vector2){0, 0}, 70, SKYBLUE, PURPLE), "Potion", "Items", true },
        { CreateTreasureChest((Vector2){0, 0}, 70, BROWN, GOLD, true), "Treasure Chest", "Items", true },
        { CreateCannon((Vector2){0, 0}, 70, DARKGRAY, BLACK), "Cannon", "Items", true },
        
        // Magic
        { CreateYellowStar((Vector2){0, 0}, 70), "Star", "Magic", true },
        { CreateLightningBolt((Vector2){0, 0}, 70, YELLOW), "Lightning", "Magic", false },
        { CreateExplosion((Vector2){0, 0}, 70, ORANGE, YELLOW), "Explosion", "Magic", true },
        { CreatePortal((Vector2){0, 0}, 70, PURPLE, BLUE), "Portal", "Magic", true },
        
        // UI
        { CreateButton((Vector2){0, 0}, (Vector2){80, 40}, SKYBLUE, BLUE), "Button", "UI", false },
        { CreateHealthBar((Vector2){0, 0}, 100, 0.75f, RED, GREEN), "Health Bar", "UI", false }
    };
    
    // Debug: Print info about the rock sprite
    for (int i = 0; i < sizeof(sprites) / sizeof(sprites[0]); i++) {
        if (strcmp(sprites[i].name, "Rock") == 0) {
            printf("DEBUG: Rock sprite found at index %d with %d shapes\n", 
                   i, sprites[i].sprite->shapeCount);
            break;
        }
    }
    
    // Debug: Print info about the star sprite
    for (int i = 0; i < sizeof(sprites) / sizeof(sprites[0]); i++) {
        if (strcmp(sprites[i].name, "Star") == 0) {
            printf("DEBUG: Star sprite found at index %d with %d shapes\n", 
                   i, sprites[i].sprite->shapeCount);
            if (sprites[i].sprite->shapeCount > 0) {
                printf("DEBUG: Star visibility: %d\n", sprites[i].sprite->visible);
            }
            break;
        }
    }
    
    const int spriteCount = sizeof(sprites) / sizeof(sprites[0]);
    const int gridRows = (spriteCount + gridCols - 1) / gridCols; // Calculate number of rows needed
    const int totalHeight = gridRows * cellHeight;
    
    // Animation variables
    float animTime = 0.0f;
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        float deltaTime = GetFrameTime();
        animTime += deltaTime;
        
        // Handle scrolling
        if (GetMouseWheelMove() != 0)
            scrollY -= GetMouseWheelMove() * scrollSpeed * deltaTime;
        
        // Limit scrolling
        if (scrollY < 0) scrollY = 0;
        if (scrollY > totalHeight - screenHeight + 50) 
            scrollY = totalHeight - screenHeight + 50;
        
        // Update sprite animations
        for (int i = 0; i < spriteCount; i++) {
            if (sprites[i].animated) {
                // Calculate grid position
                int col = i % gridCols;
                int row = i / gridCols;
                float x = col * cellWidth + cellWidth/2 + cellPadding;
                float y = row * cellHeight + cellHeight/2 + cellPadding - scrollY;
                
                // Only animate sprites that are visible on screen
                if (y >= -cellHeight && y <= screenHeight + cellHeight) {
                    if (i == 0) { // Simple character (rotation)
                        SetSpriteRotation(sprites[i].sprite, sinf(animTime) * 15.0f);
                    } else if (i == 1) { // Robot (rotation)
                        SetSpriteRotation(sprites[i].sprite, -sinf(animTime * 0.8f) * 10.0f);
                    } else if (i == 2) { // Animal (scaling)
                        float scale = 0.9f + 0.2f * sinf(animTime * 2.0f);
                        SetSpriteScale(sprites[i].sprite, scale);
                    } else if (i == 3) { // Ghost (vertical movement)
                        Vector2 pos = { 0, sinf(animTime * 3.0f) * 10.0f };
                        SetSpritePosition(sprites[i].sprite, pos);
                    } else if (strcmp(sprites[i].name, "Star") == 0) { // Star (rotation + scaling)
                        SetSpriteRotation(sprites[i].sprite, animTime * 30.0f);
                        float scale = 0.8f + 0.3f * sinf(animTime * 3.0f);
                        SetSpriteScale(sprites[i].sprite, scale);
                    } else if (strcmp(sprites[i].name, "Soldier") == 0) { // Soldier (slight marching motion)
                        Vector2 pos = { sinf(animTime * 2.0f) * 5.0f, 0 };
                        SetSpritePosition(sprites[i].sprite, pos);
                        SetSpriteRotation(sprites[i].sprite, sinf(animTime * 4.0f) * 5.0f);
                    } else if (strcmp(sprites[i].name, "Zombie") == 0) { // Zombie (shambling motion)
                        Vector2 pos = { sinf(animTime * 1.0f) * 8.0f, sinf(animTime * 2.0f) * 3.0f };
                        SetSpritePosition(sprites[i].sprite, pos);
                        SetSpriteRotation(sprites[i].sprite, sinf(animTime * 0.5f) * 7.0f);
                    } else if (strcmp(sprites[i].name, "Airplane") == 0) { // Airplane (banking motion)
                        SetSpriteRotation(sprites[i].sprite, sinf(animTime * 1.5f) * 10.0f);
                        Vector2 pos = { 0, sinf(animTime * 2.0f) * 8.0f };
                        SetSpritePosition(sprites[i].sprite, pos);
                    } else if (strcmp(sprites[i].name, "Explosion") == 0) { // Explosion (pulsing)
                        float scale = 0.9f + 0.3f * sinf(animTime * 5.0f);
                        SetSpriteScale(sprites[i].sprite, scale);
                        SetSpriteRotation(sprites[i].sprite, animTime * 20.0f);
                    } else if (strcmp(sprites[i].name, "UFO") == 0) { // UFO (hovering motion)
                        Vector2 pos = { sinf(animTime * 1.5f) * 5.0f, sinf(animTime * 2.5f) * 8.0f };
                        SetSpritePosition(sprites[i].sprite, pos);
                        float scale = 0.95f + 0.05f * sinf(animTime * 4.0f);
                        SetSpriteScale(sprites[i].sprite, scale);
                    } else if (strcmp(sprites[i].name, "Dragon") == 0) { // Dragon (wing flapping)
                        float scale = 0.95f + 0.1f * sinf(animTime * 3.0f);
                        SetSpriteScale(sprites[i].sprite, scale);
                        SetSpriteRotation(sprites[i].sprite, sinf(animTime * 0.8f) * 5.0f);
                    } else if (strcmp(sprites[i].name, "Wizard") == 0) { // Wizard (magical effect)
                        Vector2 pos = { 0, sinf(animTime * 1.5f) * 5.0f };
                        SetSpritePosition(sprites[i].sprite, pos);
                        SetSpriteRotation(sprites[i].sprite, sinf(animTime * 1.0f) * 5.0f);
                    } else if (strcmp(sprites[i].name, "Treasure Chest") == 0) { // Treasure chest (slight bouncing)
                        Vector2 pos = { 0, sinf(animTime * 3.0f) * 3.0f };
                        SetSpritePosition(sprites[i].sprite, pos);
                    } else if (strcmp(sprites[i].name, "Portal") == 0) { // Portal (spinning)
                        SetSpriteRotation(sprites[i].sprite, animTime * 30.0f);
                        float scale = 0.9f + 0.15f * sinf(animTime * 2.0f);
                        SetSpriteScale(sprites[i].sprite, scale);
                    } else if (strcmp(sprites[i].name, "Snowman") == 0) { // Snowman (slight sway)
                        SetSpriteRotation(sprites[i].sprite, sinf(animTime * 0.5f) * 3.0f);
                    } else if (strcmp(sprites[i].name, "Potion") == 0) { // Potion (bubbling effect)
                        Vector2 pos = { sinf(animTime * 2.0f) * 3.0f, sinf(animTime * 3.0f) * 2.0f };
                        SetSpritePosition(sprites[i].sprite, pos);
                    } else if (strcmp(sprites[i].name, "Cannon") == 0) { // Cannon (recoil effect)
                        Vector2 pos = { -sinf(animTime * 3.0f) * fmaxf(0, sinf(animTime * 3.0f)) * 8.0f, 0 };
                        SetSpritePosition(sprites[i].sprite, pos);
                        SetSpriteRotation(sprites[i].sprite, sinf(animTime * 3.0f) * 3.0f);
                    }
                }
            }
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            // Draw sprites in a grid
            for (int i = 0; i < spriteCount; i++) {
                int col = i % gridCols;
                int row = i / gridCols;
                
                float x = col * cellWidth + cellWidth/2 + cellPadding;
                float y = row * cellHeight + cellHeight/2 + cellPadding - scrollY;
                
                // Only draw sprites that are visible on screen
                if (y >= -cellHeight && y <= screenHeight + cellHeight) {
                    // Draw cell background based on category
                    Rectangle cellRect = { 
                        col * cellWidth + cellPadding/2, 
                        row * cellHeight + cellPadding/2 - scrollY, 
                        cellWidth - cellPadding, 
                        cellHeight - cellPadding 
                    };
                    
                    Color categoryColor;
                    if (strcmp(sprites[i].category, "Characters") == 0)
                        categoryColor = (Color){ 230, 240, 255, 255 }; // Light blue
                    else if (strcmp(sprites[i].category, "Environment") == 0)
                        categoryColor = (Color){ 230, 255, 230, 255 }; // Light green
                    else
                        categoryColor = (Color){ 255, 240, 230, 255 }; // Light orange
                    
                    DrawRectangleRec(cellRect, categoryColor);
                    DrawRectangleLinesEx(cellRect, 1, LIGHTGRAY);
                    
                    // Position sprite in cell
                    SetSpritePosition(sprites[i].sprite, (Vector2){ x, y });
                    
                    // Draw the sprite
                    DrawSprite(sprites[i].sprite);
                    
                    // Draw sprite name
                    DrawText(sprites[i].name, 
                        col * cellWidth + cellPadding, 
                        row * cellHeight + cellHeight - 40 - scrollY, 
                        16, DARKGRAY);
                    
                    // Draw category
                    DrawText(sprites[i].category, 
                        col * cellWidth + cellPadding, 
                        row * cellHeight + cellHeight - 20 - scrollY, 
                        13, GRAY);
                }
            }
            
            // Draw instructions
            DrawRectangle(0, 0, screenWidth, 40, Fade(RAYWHITE, 0.8f));
            DrawText("Use mouse wheel to scroll through the sprite gallery", 20, 10, 20, DARKGRAY);
            DrawFPS(screenWidth - 100, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Free all sprites
    for (int i = 0; i < spriteCount; i++) {
        FreeSprite(sprites[i].sprite);
    }
    
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
} 