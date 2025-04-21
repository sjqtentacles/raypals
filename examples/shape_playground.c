/*******************************************************************************************
*
*   RayPals [Shape Playground] - Interactive demo with customizable shapes
*
*   This example has been created using raylib 4.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2023 RayPals Team
*
********************************************************************************************/

#include "raylib.h"
#include "raypals.h"
#include "raymath.h"
#include <stdlib.h>

#define MAX_SHAPES 20

typedef enum {
    SHAPE_SQUARE,
    SHAPE_CIRCLE,
    SHAPE_TRIANGLE,
    SHAPE_STAR 
} ShapeType;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "RayPals - Shape Playground");

    // Initialize shape arrays
    RayPals2DShape* shapes[MAX_SHAPES] = { 0 };
    int shapeCount = 0;
    
    // User interface variables
    ShapeType currentShapeType = SHAPE_SQUARE;
    Color currentColor = RED;
    float currentSize = 50.0f;
    bool currentFilled = true;
    
    bool addMode = true;  // Add mode (true) or Edit mode (false)
    int selectedShape = -1;
    
    // Control panel area
    Rectangle controlPanel = { 600, 0, 200, screenHeight };
    
    // Color options
    Color colorOptions[] = { RED, GREEN, BLUE, YELLOW, PURPLE, ORANGE, PINK, SKYBLUE };
    int colorCount = 8;
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        Vector2 mousePos = GetMousePosition();
        bool isMousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        bool isMouseInDrawingArea = mousePos.x < controlPanel.x;
        
        // Handle mouse interactions in drawing area
        if (isMouseInDrawingArea && isMousePressed) {
            if (addMode) {
                // Add a new shape if we haven't reached the limit
                if (shapeCount < MAX_SHAPES) {
                    switch (currentShapeType) {
                        case SHAPE_SQUARE:
                            shapes[shapeCount] = CreateSquare(mousePos, currentSize, currentColor);
                            break;
                        case SHAPE_CIRCLE:
                            shapes[shapeCount] = CreateCircle(mousePos, currentSize/2, currentColor);
                            break;
                        case SHAPE_TRIANGLE:
                            shapes[shapeCount] = CreateTriangle(mousePos, currentSize, currentColor);
                            break;
                        case SHAPE_STAR:
                            shapes[shapeCount] = CreateStar(mousePos, currentSize, 5, currentColor);
                            break;
                    }
                    
                    // Set filled property
                    shapes[shapeCount]->filled = currentFilled;
                    
                    shapeCount++;
                }
            } else {
                // Selection mode - try to select a shape under the mouse pointer
                selectedShape = -1;
                
                // Check shapes in reverse order (top-most first)
                for (int i = shapeCount - 1; i >= 0; i--) {
                    // Simple collision check - just using distance for all shapes
                    float distance = Vector2Distance(mousePos, shapes[i]->position);
                    if (distance < shapes[i]->size.x / 2) {
                        selectedShape = i;
                        break;
                    }
                }
            }
        }
        
        // Handle selected shape movement with arrow keys
        if (selectedShape >= 0 && !addMode) {
            if (IsKeyDown(KEY_RIGHT)) shapes[selectedShape]->position.x += 2.0f;
            if (IsKeyDown(KEY_LEFT)) shapes[selectedShape]->position.x -= 2.0f;
            if (IsKeyDown(KEY_DOWN)) shapes[selectedShape]->position.y += 2.0f;
            if (IsKeyDown(KEY_UP)) shapes[selectedShape]->position.y -= 2.0f;
            
            // Rotate with R key
            if (IsKeyDown(KEY_R)) shapes[selectedShape]->rotation += 2.0f;
            
            // Delete selected shape with Delete key
            if (IsKeyPressed(KEY_DELETE) || IsKeyPressed(KEY_BACKSPACE)) {
                // Free shape memory
                FreeShape(shapes[selectedShape]);
                
                // Shift all shapes after this one
                for (int i = selectedShape; i < shapeCount - 1; i++) {
                    shapes[i] = shapes[i + 1];
                }
                
                shapeCount--;
                selectedShape = -1;
            }
        }
        
        // Handle control panel interactions
        if (!isMouseInDrawingArea && isMousePressed) {
            // Shape type buttons (at top of panel)
            for (int i = 0; i < 4; i++) {
                Rectangle buttonRect = { controlPanel.x + 10, 40 + i * 40, 180, 30 };
                if (CheckCollisionPointRec(mousePos, buttonRect)) {
                    currentShapeType = (ShapeType)i;
                }
            }
            
            // Size control buttons
            Rectangle sizeDownBtn = { controlPanel.x + 10, 220, 80, 30 };
            Rectangle sizeUpBtn = { controlPanel.x + 110, 220, 80, 30 };
            
            if (CheckCollisionPointRec(mousePos, sizeDownBtn) && currentSize > 20) {
                currentSize -= 10;
            }
            if (CheckCollisionPointRec(mousePos, sizeUpBtn) && currentSize < 150) {
                currentSize += 10;
            }
            
            // Filled/outline toggle
            Rectangle filledBtn = { controlPanel.x + 10, 280, 180, 30 };
            if (CheckCollisionPointRec(mousePos, filledBtn)) {
                currentFilled = !currentFilled;
            }
            
            // Color selection
            int colorButtonSize = 40;
            int colorPerRow = 4;
            for (int i = 0; i < colorCount; i++) {
                int row = i / colorPerRow;
                int col = i % colorPerRow;
                Rectangle colorRect = { 
                    controlPanel.x + 10 + col * (colorButtonSize + 5), 
                    340 + row * (colorButtonSize + 5), 
                    colorButtonSize, 
                    colorButtonSize 
                };
                
                if (CheckCollisionPointRec(mousePos, colorRect)) {
                    currentColor = colorOptions[i];
                }
            }
            
            // Mode toggle button
            Rectangle modeRect = { controlPanel.x + 10, 450, 180, 30 };
            if (CheckCollisionPointRec(mousePos, modeRect)) {
                addMode = !addMode;
                selectedShape = -1;
            }
            
            // Clear button
            Rectangle clearRect = { controlPanel.x + 10, 500, 180, 30 };
            if (CheckCollisionPointRec(mousePos, clearRect)) {
                // Free all shapes
                for (int i = 0; i < shapeCount; i++) {
                    FreeShape(shapes[i]);
                }
                shapeCount = 0;
                selectedShape = -1;
            }
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            // Draw shapes
            for (int i = 0; i < shapeCount; i++) {
                Draw2DShape(shapes[i]);
                
                // Draw selection indicator if this shape is selected
                if (i == selectedShape) {
                    float size = shapes[i]->size.x;
                    DrawCircleLines(shapes[i]->position.x, shapes[i]->position.y, size/2 + 5, RED);
                }
            }
            
            // Draw control panel background
            DrawRectangleRec(controlPanel, LIGHTGRAY);
            DrawLine(controlPanel.x, 0, controlPanel.x, screenHeight, DARKGRAY);
            
            // Draw controls
            DrawText("Shape Playground", controlPanel.x + 10, 10, 20, DARKGRAY);
            
            // Shape type buttons
            const char* shapeNames[] = { "Square", "Circle", "Triangle", "Star" };
            for (int i = 0; i < 4; i++) {
                Rectangle buttonRect = { controlPanel.x + 10, 40 + i * 40, 180, 30 };
                DrawRectangleRec(buttonRect, (i == currentShapeType) ? SKYBLUE : GRAY);
                DrawText(shapeNames[i], buttonRect.x + 10, buttonRect.y + 5, 20, BLACK);
            }
            
            // Size controls
            DrawText("Size:", controlPanel.x + 10, 190, 20, BLACK);
            Rectangle sizeDownBtn = { controlPanel.x + 10, 220, 80, 30 };
            Rectangle sizeUpBtn = { controlPanel.x + 110, 220, 80, 30 };
            DrawRectangleRec(sizeDownBtn, GRAY);
            DrawRectangleRec(sizeUpBtn, GRAY);
            DrawText("-", sizeDownBtn.x + 35, sizeDownBtn.y + 5, 20, BLACK);
            DrawText("+", sizeUpBtn.x + 35, sizeUpBtn.y + 5, 20, BLACK);
            DrawText(TextFormat("%d", (int)currentSize), controlPanel.x + 100, 190, 20, BLACK);
            
            // Filled/outline toggle
            DrawText("Style:", controlPanel.x + 10, 260, 20, BLACK);
            Rectangle filledBtn = { controlPanel.x + 10, 280, 180, 30 };
            DrawRectangleRec(filledBtn, GRAY);
            DrawText(currentFilled ? "Filled" : "Outline", filledBtn.x + 10, filledBtn.y + 5, 20, BLACK);
            
            // Color selection
            DrawText("Color:", controlPanel.x + 10, 320, 20, BLACK);
            int colorButtonSize = 40;
            int colorPerRow = 4;
            for (int i = 0; i < colorCount; i++) {
                int row = i / colorPerRow;
                int col = i % colorPerRow;
                Rectangle colorRect = { 
                    controlPanel.x + 10 + col * (colorButtonSize + 5), 
                    340 + row * (colorButtonSize + 5), 
                    colorButtonSize, 
                    colorButtonSize 
                };
                
                DrawRectangleRec(colorRect, colorOptions[i]);
                
                // Draw indicator around current color
                if (ColorToInt(colorOptions[i]) == ColorToInt(currentColor)) {
                    DrawRectangleLinesEx(colorRect, 2, BLACK);
                }
            }
            
            // Mode toggle
            Rectangle modeRect = { controlPanel.x + 10, 450, 180, 30 };
            DrawRectangleRec(modeRect, GRAY);
            DrawText(addMode ? "Add Mode" : "Edit Mode", modeRect.x + 10, modeRect.y + 5, 20, BLACK);
            
            // Clear button
            Rectangle clearRect = { controlPanel.x + 10, 500, 180, 30 };
            DrawRectangleRec(clearRect, RED);
            DrawText("Clear All", clearRect.x + 10, clearRect.y + 5, 20, WHITE);
            
            // Instructions for edit mode
            if (!addMode) {
                DrawText("Edit Controls:", controlPanel.x + 10, 540, 15, DARKGRAY);
                DrawText("  Arrows: Move", controlPanel.x + 10, 560, 15, DARKGRAY);
                DrawText("  R: Rotate", controlPanel.x + 10, 575, 15, DARKGRAY);
                DrawText("  Delete: Remove", controlPanel.x + 10, 590, 15, DARKGRAY);
            }

            DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Free all shapes
    for (int i = 0; i < shapeCount; i++) {
        FreeShape(shapes[i]);
    }
    
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
} 