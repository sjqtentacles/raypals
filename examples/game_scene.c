/*******************************************************************************************
*
*   RayPals [Game Scene] - Example showing a complete game scene using the expanded sprite library
*
*   This example has been created using raylib 4.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2023 RayPals Team
*
********************************************************************************************/

#include "raylib.h"
#include "raypals.h"
#include <math.h>

// Game object collection
typedef struct {
    RayPalsSprite* player;
    RayPalsSprite* houses[3];
    RayPalsSprite* trees[5];
    RayPalsSprite* bushes[8];
    RayPalsSprite* rocks[4];
    RayPalsSprite* clouds[3];
    RayPalsSprite* enemies[4];
    RayPalsSprite* healthBar;
} GameScene;

// Function to free all sprites in a scene
void FreeGameScene(GameScene* scene) {
    FreeSprite(scene->player);
    
    for (int i = 0; i < 3; i++) FreeSprite(scene->houses[i]);
    for (int i = 0; i < 5; i++) FreeSprite(scene->trees[i]);
    for (int i = 0; i < 8; i++) FreeSprite(scene->bushes[i]);
    for (int i = 0; i < 4; i++) FreeSprite(scene->rocks[i]);
    for (int i = 0; i < 3; i++) FreeSprite(scene->clouds[i]);
    for (int i = 0; i < 4; i++) FreeSprite(scene->enemies[i]);
    
    FreeSprite(scene->healthBar);
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "RayPals - Game Scene Example");

    // Create a complete game scene
    GameScene scene = { 0 };
    
    // Player (robot character)
    scene.player = CreateRobotCharacter((Vector2){ 400, 300 }, 60, BLUE, WHITE);
    
    // Houses
    scene.houses[0] = CreateHouse((Vector2){ 650, 350 }, 120, LIGHTGRAY, RED);
    scene.houses[1] = CreateHouse((Vector2){ 500, 400 }, 100, BEIGE, ORANGE);
    scene.houses[2] = CreateHouse((Vector2){ 700, 450 }, 80, WHITE, DARKBLUE);
    
    // Trees
    scene.trees[0] = CreateSimpleTree((Vector2){ 100, 400 }, 100, BROWN, GREEN);
    scene.trees[1] = CreateSimpleTree((Vector2){ 180, 450 }, 80, BROWN, DARKGREEN);
    scene.trees[2] = CreateSimpleTree((Vector2){ 300, 420 }, 90, BROWN, GREEN);
    scene.trees[3] = CreateSimpleTree((Vector2){ 50, 480 }, 70, BROWN, DARKGREEN);
    scene.trees[4] = CreateSimpleTree((Vector2){ 250, 500 }, 110, BROWN, DARKGREEN);
    
    // Bushes
    for (int i = 0; i < 8; i++) {
        scene.bushes[i] = CreateBush(
            (Vector2){ 100 + i * 90, 520 + GetRandomValue(-20, 20) }, 
            30 + GetRandomValue(0, 20), 
            GREEN
        );
    }
    
    // Rocks
    for (int i = 0; i < 4; i++) {
        scene.rocks[i] = CreateRock(
            (Vector2){ 150 + i * 170, 500 + GetRandomValue(-10, 10) }, 
            20 + GetRandomValue(0, 10), 
            GRAY
        );
    }
    
    // Clouds
    scene.clouds[0] = CreateCloud((Vector2){ 100, 100 }, 60, WHITE);
    scene.clouds[1] = CreateCloud((Vector2){ 300, 80 }, 80, WHITE);
    scene.clouds[2] = CreateCloud((Vector2){ 600, 120 }, 70, WHITE);
    
    // Enemies (ghosts)
    scene.enemies[0] = CreateGhost((Vector2){ 200, 200 }, 40, VIOLET);
    scene.enemies[1] = CreateGhost((Vector2){ 650, 250 }, 45, PINK);
    scene.enemies[2] = CreateGhost((Vector2){ 450, 180 }, 38, PURPLE);
    scene.enemies[3] = CreateGhost((Vector2){ 300, 280 }, 42, VIOLET);
    
    // UI - Health bar
    scene.healthBar = CreateHealthBar((Vector2){ 100, 40 }, 150, 0.8f, DARKGRAY, RED);
    
    // Cloud and enemy movement speeds
    float cloudSpeeds[3] = { 30.0f, 20.0f, 25.0f };
    Vector2 enemySpeeds[4] = {
        (Vector2){ 20.0f, 15.0f },
        (Vector2){ -25.0f, 10.0f },
        (Vector2){ 15.0f, -20.0f },
        (Vector2){ -10.0f, -15.0f }
    };

    float playerHealth = 0.8f;
    float playerSpeed = 120.0f;
    Camera2D camera = { 0 };
    camera.zoom = 1.0f;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        float deltaTime = GetFrameTime();
        
        // Move player based on input
        if (IsKeyDown(KEY_RIGHT)) SetSpritePosition(scene.player, (Vector2){ scene.player->position.x + playerSpeed*deltaTime, scene.player->position.y });
        if (IsKeyDown(KEY_LEFT)) SetSpritePosition(scene.player, (Vector2){ scene.player->position.x - playerSpeed*deltaTime, scene.player->position.y });
        if (IsKeyDown(KEY_UP)) SetSpritePosition(scene.player, (Vector2){ scene.player->position.x, scene.player->position.y - playerSpeed*deltaTime });
        if (IsKeyDown(KEY_DOWN)) SetSpritePosition(scene.player, (Vector2){ scene.player->position.x, scene.player->position.y + playerSpeed*deltaTime });
        
        // Update camera to follow player
        camera.target = scene.player->position;
        camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
        
        // Move clouds
        for (int i = 0; i < 3; i++) {
            SetSpritePosition(scene.clouds[i], (Vector2){ scene.clouds[i]->position.x + cloudSpeeds[i]*deltaTime, scene.clouds[i]->position.y });
            if (scene.clouds[i]->position.x > screenWidth + 100) 
                SetSpritePosition(scene.clouds[i], (Vector2){ -100, scene.clouds[i]->position.y });
        }
        
        // Move enemies
        for (int i = 0; i < 4; i++) {
            Vector2 newPos = {
                scene.enemies[i]->position.x + enemySpeeds[i].x * deltaTime,
                scene.enemies[i]->position.y + enemySpeeds[i].y * deltaTime
            };
            
            // Bounce off screen edges
            if (newPos.x < 0 || newPos.x > screenWidth) enemySpeeds[i].x *= -1;
            if (newPos.y < 0 || newPos.y > screenHeight) enemySpeeds[i].y *= -1;
            
            SetSpritePosition(scene.enemies[i], newPos);
        }
        
        // Update health bar for demo
        if (IsKeyPressed(KEY_H)) {
            playerHealth -= 0.1f;
            if (playerHealth < 0) playerHealth = 0;
            
            // Update health bar
            FreeSprite(scene.healthBar);
            scene.healthBar = CreateHealthBar((Vector2){ 100, 40 }, 150, playerHealth, DARKGRAY, RED);
        }
        
        // Rotate trees slightly for a wind effect
        for (int i = 0; i < 5; i++) {
            RotateSprite(scene.trees[i], deltaTime, sinf(GetTime() + i) * 2.0f);
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(SKYBLUE);
            
            // Draw clouds (in screen space, not affected by camera)
            for (int i = 0; i < 3; i++) DrawSprite(scene.clouds[i]);
            
            // Begin camera mode for the game world
            BeginMode2D(camera);
            
            // Draw background
            DrawRectangle(-1000, 550, 3000, 1000, DARKGREEN);
            
            // Draw game objects (back to front)
            for (int i = 0; i < 5; i++) DrawSprite(scene.trees[i]);
            for (int i = 0; i < 3; i++) DrawSprite(scene.houses[i]);
            for (int i = 0; i < 8; i++) DrawSprite(scene.bushes[i]);
            for (int i = 0; i < 4; i++) DrawSprite(scene.rocks[i]);
            
            // Draw enemies
            for (int i = 0; i < 4; i++) DrawSprite(scene.enemies[i]);
            
            // Draw player
            DrawSprite(scene.player);
            
            EndMode2D();
            
            // Draw UI elements (in screen space, not affected by camera)
            DrawSprite(scene.healthBar);
            DrawText("Health", 50, 35, 20, BLACK);
            
            // Draw instructions
            DrawText("Use arrow keys to move", 10, 10, 20, BLACK);
            DrawText("Press H to decrease health", screenWidth - 300, 10, 20, BLACK);
            DrawFPS(screenWidth - 80, screenHeight - 30);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Free all game objects
    FreeGameScene(&scene);
    
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
} 