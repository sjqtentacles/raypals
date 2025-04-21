/*******************************************************************************************
*
*   RayPals [Shape Animation] - Example demonstrating shape animations
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

    InitWindow(screenWidth, screenHeight, "RayPals - Shape Animation Example");

    // Create our shapes with equal vertical positioning
    RayPals2DShape* pulsatingCircle = CreateCircle((Vector2){ 150, 300 }, 50, RED);
    RayPals2DShape* rotatingSquare = CreateSquare((Vector2){ 350, 300 }, 80, BLUE);
    RayPals2DShape* colorChangingShape = CreateCircle((Vector2){ 550, 300 }, 60, MAGENTA);
    RayPals2DShape* combinedAnimationTriangle = CreateTriangle((Vector2){ 700, 300 }, 80, PURPLE);
    
    colorChangingShape->filled = true;
    colorChangingShape->visible = true;
    
    // Create animations
    RayPalsAnimation pulseAnimation = {0};
    pulseAnimation.isAnimated = true;
    pulseAnimation.animationSpeed = 2.0f;
    pulseAnimation.animationTime = 0.0f;  // Initialize animation time
    pulseAnimation.scaleMin = 0.7f;
    pulseAnimation.scaleMax = 1.3f;
    pulseAnimation.pingPong = true;
    
    RayPalsAnimation rotationAnimation = {0};
    rotationAnimation.isAnimated = true;
    rotationAnimation.animationSpeed = 1.0f;
    rotationAnimation.animationTime = 0.0f;  // Initialize animation time
    rotationAnimation.rotationSpeed = 90.0f; // 90 degrees per second
    
    RayPalsAnimation colorAnimation = {0};
    colorAnimation.isAnimated = true;
    colorAnimation.animationSpeed = 1.0f;
    colorAnimation.animationTime = 0.0f;  // Initialize animation time
    colorAnimation.colorStart = MAGENTA;
    colorAnimation.colorEnd = YELLOW;  // High contrast color change
    colorAnimation.pingPong = true;
    colorAnimation.rotationSpeed = 0.0f;  // No rotation for now
    colorAnimation.scaleMin = 1.0f;      // No scaling for now
    colorAnimation.scaleMax = 1.0f;
    
    RayPalsAnimation combinedAnimation = {0};
    combinedAnimation.isAnimated = true;
    combinedAnimation.animationSpeed = 1.8f;
    combinedAnimation.animationTime = 0.0f;  // Initialize animation time
    combinedAnimation.rotationSpeed = 45.0f;
    combinedAnimation.scaleMin = 0.8f;
    combinedAnimation.scaleMax = 1.2f;
    combinedAnimation.colorStart = PURPLE;
    combinedAnimation.colorEnd = PINK;
    combinedAnimation.pingPong = true;
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        float deltaTime = GetFrameTime();
        
        // Update animations
        UpdateShapeAnimation(pulsatingCircle, &pulseAnimation, deltaTime);
        UpdateShapeAnimation(rotatingSquare, &rotationAnimation, deltaTime);
        UpdateShapeAnimation(colorChangingShape, &colorAnimation, deltaTime);
        UpdateShapeAnimation(combinedAnimationTriangle, &combinedAnimation, deltaTime);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            // Draw 2D shapes
            Draw2DShape(pulsatingCircle);
            Draw2DShape(rotatingSquare);
            Draw2DShape(colorChangingShape);
            Draw2DShape(combinedAnimationTriangle);
            
            // Draw labels
            DrawText("Pulsating", 110, 380, 20, DARKGRAY);
            DrawText("Circle", 130, 400, 20, DARKGRAY);
            
            DrawText("Rotating", 310, 380, 20, DARKGRAY);
            DrawText("Square", 325, 400, 20, DARKGRAY);
            
            DrawText("Color Changing", 500, 380, 20, DARKGRAY);
            DrawText("Circle", 530, 400, 20, DARKGRAY);
            
            DrawText("Combined", 650, 380, 20, DARKGRAY);
            DrawText("Animation", 650, 400, 20, DARKGRAY);
            
            DrawFPS(screenWidth - 100, 20);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Free shapes
    FreeShape(pulsatingCircle);
    FreeShape(rotatingSquare);
    FreeShape(colorChangingShape);
    FreeShape(combinedAnimationTriangle);
    
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
} 