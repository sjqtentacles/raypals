/*******************************************************************************************
*
*   RayPals [3D Robot Example] - Example showing how to create and display 3D robots
*
*   This example has been created using raylib 4.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2024 RayPals Team
*
********************************************************************************************/

#include "raylib.h"
#include "raypals.h"
#include <math.h>

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "RayPals - 3D Robot Example");

    // Define camera
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 5.0f, 5.0f, 5.0f };    // Camera position
    camera.target = (Vector3){ 0.0f, 1.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    // Camera orbit variables
    float cameraAngle = 0.0f;                           // Current camera angle (degrees)
    float cameraRadius = 7.0f;                          // Distance from center
    float cameraHeight = 3.0f;                          // Fixed height
    float rotationSpeed = 0.5f;                         // Degrees per frame
    bool rotationPaused = false;                        // Flag to pause/resume rotation

    // Create the 3D robot
    RayPals3DSprite* robot = Create3DRobot(
        (Vector3){ 0.0f, 1.0f, 0.0f },                  // Position
        2.0f,                                           // Size
        RED,                                            // Body color
        YELLOW                                          // Detail color
    );

    // Create additional robots with different colors/positions
    RayPals3DSprite* robot2 = Create3DRobot(
        (Vector3){ 3.0f, 1.0f, 3.0f },                  // Position
        1.5f,                                           // Size
        BLUE,                                           // Body color
        WHITE                                           // Detail color
    );

    RayPals3DSprite* robot3 = Create3DRobot(
        (Vector3){ -3.0f, 1.0f, -3.0f },                // Position
        1.8f,                                           // Size
        GREEN,                                          // Body color
        ORANGE                                          // Detail color
    );

    // Robot animation variables
    Vector3 rotationSpeed1 = { 0.0f, 30.0f, 0.0f };     // Rotation speed in degrees per second
    Vector3 rotationSpeed2 = { 10.0f, 20.0f, 5.0f };    // Different rotation for robot2
    Vector3 rotationSpeed3 = { 0.0f, -40.0f, 0.0f };    // Different rotation for robot3

    SetTargetFPS(60);                                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        float deltaTime = GetFrameTime();

        // Check for toggle camera rotation
        if (IsKeyPressed(KEY_SPACE)) {
            rotationPaused = !rotationPaused;
        }
        
        // Update camera position if rotation is not paused
        if (!rotationPaused) {
            cameraAngle += rotationSpeed;
            if (cameraAngle >= 360.0f) cameraAngle -= 360.0f;
            
            // Calculate new camera position based on angle
            camera.position.x = cosf(cameraAngle * DEG2RAD) * cameraRadius;
            camera.position.z = sinf(cameraAngle * DEG2RAD) * cameraRadius;
            camera.position.y = cameraHeight;
        }

        // Process robot controls
        if (IsKeyDown(KEY_UP)) {
            Vector3 pos = { 0, 0, -0.1f };
            Set3DSpritePosition(robot, (Vector3){ 
                robot->position.x + pos.x,
                robot->position.y + pos.y, 
                robot->position.z + pos.z
            });
        }
        if (IsKeyDown(KEY_DOWN)) {
            Vector3 pos = { 0, 0, 0.1f };
            Set3DSpritePosition(robot, (Vector3){ 
                robot->position.x + pos.x,
                robot->position.y + pos.y, 
                robot->position.z + pos.z
            });
        }
        if (IsKeyDown(KEY_LEFT)) {
            Vector3 pos = { -0.1f, 0, 0 };
            Set3DSpritePosition(robot, (Vector3){ 
                robot->position.x + pos.x,
                robot->position.y + pos.y, 
                robot->position.z + pos.z
            });
        }
        if (IsKeyDown(KEY_RIGHT)) {
            Vector3 pos = { 0.1f, 0, 0 };
            Set3DSpritePosition(robot, (Vector3){ 
                robot->position.x + pos.x,
                robot->position.y + pos.y, 
                robot->position.z + pos.z
            });
        }

        // Rotate robots
        Rotate3DSprite(robot, deltaTime, rotationSpeed1);
        Rotate3DSprite(robot2, deltaTime, rotationSpeed2);
        Rotate3DSprite(robot3, deltaTime, rotationSpeed3);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            BeginMode3D(camera);
                
                // Draw grid for reference
                DrawGrid(10, 1.0f);
                
                // Draw ground plane
                DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 20.0f, 20.0f }, LIGHTGRAY);
                
                // Draw robots
                Draw3DSprite(robot, camera);
                Draw3DSprite(robot2, camera);
                Draw3DSprite(robot3, camera);
                
            EndMode3D();

            // Draw UI
            DrawRectangle(0, 0, screenWidth, 40, (Color){ 0, 0, 0, 120 });
            DrawText("3D Robot Example", 10, 10, 20, WHITE);
            DrawFPS(700, 10);
            
            // Instructions
            DrawRectangle(0, screenHeight - 80, screenWidth, 80, (Color){ 0, 0, 0, 120 });
            DrawText("Press SPACE to pause/resume camera rotation", 10, screenHeight - 70, 20, WHITE);
            DrawText("Use ARROW KEYS to move the main robot", 10, screenHeight - 40, 20, WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Free all robots
    Free3DSprite(robot);
    Free3DSprite(robot2);
    Free3DSprite(robot3);
    
    CloseWindow();
    //--------------------------------------------------------------------------------------

    return 0;
} 