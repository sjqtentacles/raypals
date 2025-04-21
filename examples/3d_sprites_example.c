/*******************************************************************************************
*
*   RayPals [3D Forest Example] - Example showing a detailed 3D forest scene
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
#include <stdlib.h>

// Custom drawing functions for 3D shapes
void DrawCone(Vector3 position, float radius, float height, int segments, Color color) {
    DrawCylinderEx(position, 
                  (Vector3){position.x, position.y + height, position.z}, 
                  radius, 0.0f, 
                  segments, color);
}

void DrawConeWires(Vector3 position, float radius, float height, int segments, Color color) {
    DrawCylinderWiresEx(position, 
                      (Vector3){position.x, position.y + height, position.z}, 
                      radius, 0.0f, 
                      segments, color);
}

void DrawCylinder(Vector3 position, float radius, float radius2, float height, int segments, Color color) {
    DrawCylinderEx(position, 
                  (Vector3){position.x, position.y + height, position.z}, 
                  radius, radius2, 
                  segments, color);
}

void DrawCylinderWires(Vector3 position, float radius, float radius2, float height, int segments, Color color) {
    DrawCylinderWiresEx(position, 
                      (Vector3){position.x, position.y + height, position.z}, 
                      radius, radius2, 
                      segments, color);
}

// Simple draw function to ensure no movement
void DrawStaticScene(Camera3D camera, RayPals3DTree* trees, int treeCount, 
                     RayPals3DTree centerTree, RayPals3DShape* rock1, RayPals3DShape* rock2, 
                     bool drawWireframes) {
    ClearBackground((Color){ 135, 206, 235, 255 });  // Sky blue

    BeginMode3D(camera);
        // Draw ground
        DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 40.0f, 40.0f }, (Color){ 34, 139, 34, 255 });  // Forest green ground
        
        // Draw grass patches (circles on the ground)
        for (int i = 0; i < 30; i++) {
            DrawCircle3D(
                (Vector3){ 
                    cosf((float)i * 0.7f) * 10.0f,
                    0.01f,
                    sinf((float)i * 0.7f) * 10.0f
                },
                2.0f,
                (Vector3){ 1.0f, 0.0f, 0.0f },
                0.0f,
                (Color){ 50, 205, 50, 255 }  // Lime green
            );
        }

        // Draw all trees - ensure zero rotation
        for (int i = 0; i < treeCount; i++) {
            // Force zero rotation before drawing
            Vector3 zeroRotation = { 0.0f, 0.0f, 0.0f };
            Set3DSpriteRotation(trees[i].sprite, zeroRotation);
            
            // Draw each shape individually with manual transformations
            for (int j = 0; j < trees[i].sprite->shapeCount; j++) {
                RayPals3DShape* shape = trees[i].sprite->shapes[j];
                Vector3 pos = trees[i].sprite->position;
                
                // Apply tree position to shape position
                Vector3 finalPos = {
                    pos.x + shape->position.x,
                    pos.y + shape->position.y,
                    pos.z + shape->position.z
                };
                
                // Draw based on shape type
                if (shape->type == RAYPALS_CYLINDER) { // Trunk
                    // Draw solid shape
                    DrawCylinder(
                        finalPos,
                        shape->size.x/2,  // radius
                        shape->size.x/2,  // top radius
                        shape->size.y,    // height
                        shape->segments,
                        shape->color
                    );
                    
                    // Draw wireframe outline if requested
                    if (drawWireframes) {
                        DrawCylinderWires(
                            finalPos,
                            shape->size.x/2,  // radius
                            shape->size.x/2,  // top radius
                            shape->size.y,    // height
                            shape->segments,
                            BLACK
                        );
                    }
                } else if (shape->type == RAYPALS_CONE) { // Leaves
                    // Draw solid shape
                    DrawCone(
                        finalPos,
                        shape->size.x/2,  // radius
                        shape->size.y,    // height
                        shape->segments,
                        shape->color
                    );
                    
                    // Draw wireframe outline if requested
                    if (drawWireframes) {
                        DrawConeWires(
                            finalPos,
                            shape->size.x/2,  // radius
                            shape->size.y,    // height
                            shape->segments,
                            BLACK
                        );
                    }
                }
            }
        }

        // Draw center tree with zero rotation
        Vector3 zeroRotation = { 0.0f, 0.0f, 0.0f };
        Set3DSpriteRotation(centerTree.sprite, zeroRotation);
        
        // Draw each shape in center tree
        for (int j = 0; j < centerTree.sprite->shapeCount; j++) {
            RayPals3DShape* shape = centerTree.sprite->shapes[j];
            Vector3 pos = centerTree.sprite->position;
            
            // Apply tree position to shape position
            Vector3 finalPos = {
                pos.x + shape->position.x,
                pos.y + shape->position.y,
                pos.z + shape->position.z
            };
            
            // Draw based on shape type
            if (shape->type == RAYPALS_CYLINDER) { // Trunk
                // Draw solid shape
                DrawCylinder(
                    finalPos,
                    shape->size.x/2,  // radius
                    shape->size.x/2,  // top radius
                    shape->size.y,    // height
                    shape->segments,
                    shape->color
                );
                
                // Draw wireframe outline if requested
                if (drawWireframes) {
                    DrawCylinderWires(
                        finalPos,
                        shape->size.x/2,  // radius
                        shape->size.x/2,  // top radius
                        shape->size.y,    // height
                        shape->segments,
                        BLACK
                    );
                }
            } else if (shape->type == RAYPALS_CONE) { // Leaves
                // Draw solid shape
                DrawCone(
                    finalPos,
                    shape->size.x/2,  // radius
                    shape->size.y,    // height
                    shape->segments,
                    shape->color
                );
                
                // Draw wireframe outline if requested
                if (drawWireframes) {
                    DrawConeWires(
                        finalPos,
                        shape->size.x/2,  // radius
                        shape->size.y,    // height
                        shape->segments,
                        BLACK
                    );
                }
            }
        }

        // Draw rocks directly
        DrawSphere(rock1->position, rock1->size.x/2, rock1->color);
        if (drawWireframes) {
            DrawSphereWires(rock1->position, rock1->size.x/2, 8, 8, BLACK);
        }
        
        DrawSphere(rock2->position, rock2->size.x/2, rock2->color);
        if (drawWireframes) {
            DrawSphereWires(rock2->position, rock2->size.x/2, 8, 8, BLACK);
        }
        
        // Draw coordinate grid
        DrawGrid(10, 1.0f);
    EndMode3D();

    // Draw title and info
    DrawRectangle(0, 0, 800, 40, (Color){ 0, 0, 0, 120 });
    DrawText(drawWireframes ? "3D Forest Scene (With Wireframes)" : "3D Forest Scene", 10, 10, 20, WHITE);
    DrawFPS(700, 10);
    
    // Instructions
    DrawRectangle(0, 530, 800, 70, (Color){ 0, 0, 0, 120 });
    DrawText("Press W to toggle wireframe mode", 10, 520, 20, WHITE);
}

int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "RayPals - 3D Forest Scene");

    // Define camera with a fixed viewpoint
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 12.0f, 12.0f, 12.0f };  // Initial position
    camera.target = (Vector3){ 0.0f, 1.0f, 0.0f };       // Look slightly above ground at center
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };           // Y-up orientation
    camera.fovy = 45.0f;                                 // Field of view
    camera.projection = CAMERA_PERSPECTIVE;              // Perspective projection

    // Camera rotation variables
    float cameraAngle = 0.0f;       // Current angle around the scene
    float cameraRadius = 15.0f;     // Distance from center
    float cameraHeight = 12.0f;     // Fixed height
    float rotationSpeed = 0.2f;     // Degrees per frame
    bool rotationPaused = false;    // Flag to pause/resume rotation

    // Create multiple trees in a forest arrangement
    const int NUM_TREES = 12;
    RayPals3DTree trees[NUM_TREES];
    
    // Create trees in a circular pattern
    for (int i = 0; i < NUM_TREES; i++) {
        float angle = (float)i * (360.0f / NUM_TREES) * DEG2RAD;
        float radius = 8.0f;  // Distance from center
        float randomOffset = ((float)GetRandomValue(-100, 100) / 100.0f) * 2.0f;  // Random offset
        float scale = 0.8f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.4f;    // Random scale

        Vector3 position = {
            cosf(angle) * (radius + randomOffset),
            0.0f,
            sinf(angle) * (radius + randomOffset)
        };

        trees[i] = Create3DTree(position, scale, 
                               (Color){ 139, 69, 19, 255 },   // Saddle brown trunk
                               (Color){ 34, 139, 34, 255 });  // Forest green leaves
                               
        // Set rotation to zero immediately after creation
        Vector3 zeroRotation = { 0.0f, 0.0f, 0.0f };
        Set3DSpriteRotation(trees[i].sprite, zeroRotation);
    }

    // Create central tree
    RayPals3DTree centerTree = Create3DTree(
        (Vector3){ 0.0f, 0.0f, 0.0f }, 
        1.2f, 
        (Color){ 139, 69, 19, 255 },   // Saddle brown trunk
        (Color){ 34, 139, 34, 255 }    // Forest green leaves
    );
    
    // Set rotation to zero immediately after creation
    Vector3 zeroRotation = { 0.0f, 0.0f, 0.0f };
    Set3DSpriteRotation(centerTree.sprite, zeroRotation);

    // Create ground features
    RayPals3DShape* rock1 = CreateSphere(
        (Vector3){ 3.0f, 0.5f, -2.0f },
        0.8f,
        8,
        GRAY
    );

    RayPals3DShape* rock2 = CreateSphere(
        (Vector3){ -4.0f, 0.7f, 2.0f },
        1.0f,
        8,
        DARKGRAY
    );

    // Wireframe mode flag
    bool drawWireframes = false;
    
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) {
        // Check for toggle wireframe mode
        if (IsKeyPressed(KEY_W)) {
            drawWireframes = !drawWireframes;
        }
        
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
        
        BeginDrawing();
        
        // Draw the entire scene using our static draw function
        DrawStaticScene(camera, trees, NUM_TREES, centerTree, rock1, rock2, drawWireframes);
        
        // Additional instructions for camera control
        DrawText("Press SPACE to pause/resume camera rotation", 10, 550, 20, WHITE);
        
        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Free all trees
    for (int i = 0; i < NUM_TREES; i++) {
        Free3DTree(&trees[i]);
    }
    Free3DTree(&centerTree);
    
    // Free rocks
    Free3DShape(rock1);
    Free3DShape(rock2);
    
    CloseWindow();
    //--------------------------------------------------------------------------------------

    return 0;
} 