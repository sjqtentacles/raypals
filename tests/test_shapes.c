#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/raypals.h"

// Test function declarations
void test_square_creation();
void test_circle_creation();
void test_sprite_creation();
void test_shape_manipulation();
void test_animation();
void test_3d_robot_creation();

int main() {
    // Initialize raylib window for testing
    InitWindow(800, 600, "RayPals Tests");
    SetTargetFPS(60);

    // Run tests
    printf("Starting RayPals tests...\n");
    
    test_square_creation();
    test_circle_creation();
    test_sprite_creation();
    test_shape_manipulation();
    test_animation();
    test_3d_robot_creation();

    printf("All tests completed!\n");

    // Main game loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        // Draw test results
        DrawText("Tests completed! Check console for results.", 10, 10, 20, BLACK);
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void test_square_creation() {
    printf("\nTesting square creation...\n");
    
    // Create a square
    Vector2 position = {100, 100};
    float size = 50;
    Color color = RED;
    
    RayPals2DShape* square = CreateSquare(position, size, color);
    
    if (square == NULL) {
        printf("FAIL: Square creation failed\n");
        return;
    }
    
    // Verify square properties
    if (square->type != RAYPALS_SQUARE) {
        printf("FAIL: Square type incorrect\n");
    }
    
    if (square->position.x != position.x || square->position.y != position.y) {
        printf("FAIL: Square position incorrect\n");
    }
    
    if (square->size.x != size || square->size.y != size) {
        printf("FAIL: Square size incorrect\n");
    }
    
    if (square->color.r != color.r || square->color.g != color.g || 
        square->color.b != color.b || square->color.a != color.a) {
        printf("FAIL: Square color incorrect\n");
    }
    
    printf("PASS: Square creation test completed\n");
    FreeShape(square);
}

void test_circle_creation() {
    printf("\nTesting circle creation...\n");
    
    // Create a circle
    Vector2 position = {200, 200};
    float radius = 30;
    Color color = BLUE;
    
    RayPals2DShape* circle = CreateCircle(position, radius, color);
    
    if (circle == NULL) {
        printf("FAIL: Circle creation failed\n");
        return;
    }
    
    // Verify circle properties
    if (circle->type != RAYPALS_CIRCLE) {
        printf("FAIL: Circle type incorrect\n");
    }
    
    if (circle->position.x != position.x || circle->position.y != position.y) {
        printf("FAIL: Circle position incorrect\n");
    }
    
    if (circle->size.x != radius * 2 || circle->size.y != radius * 2) {
        printf("FAIL: Circle size incorrect\n");
    }
    
    if (circle->color.r != color.r || circle->color.g != color.g || 
        circle->color.b != color.b || circle->color.a != color.a) {
        printf("FAIL: Circle color incorrect\n");
    }
    
    printf("PASS: Circle creation test completed\n");
    FreeShape(circle);
}

void test_sprite_creation() {
    printf("\nTesting sprite creation...\n");
    
    // Create a sprite
    RayPalsSprite* sprite = CreateSprite(2);
    
    if (sprite == NULL) {
        printf("FAIL: Sprite creation failed\n");
        return;
    }
    
    // Add shapes to sprite
    Vector2 pos1 = {300, 300};
    Vector2 pos2 = {350, 350};
    
    RayPals2DShape* square = CreateSquare(pos1, 20, RED);
    RayPals2DShape* circle = CreateCircle(pos2, 15, BLUE);
    
    AddShapeToSprite(sprite, square);
    AddShapeToSprite(sprite, circle);
    
    // Verify sprite properties
    if (sprite->shapeCount != 2) {
        printf("FAIL: Sprite shape count incorrect\n");
    }
    
    if (sprite->shapes[0] != square || sprite->shapes[1] != circle) {
        printf("FAIL: Sprite shapes incorrect\n");
    }
    
    printf("PASS: Sprite creation test completed\n");
    FreeSprite(sprite);
}

void test_shape_manipulation() {
    printf("\nTesting shape manipulation...\n");
    
    // Create a shape
    Vector2 position = {400, 400};
    RayPals2DShape* shape = CreateSquare(position, 40, GREEN);
    
    // Test position setting
    Vector2 newPos = {450, 450};
    SetShapePosition(shape, newPos);
    
    if (shape->position.x != newPos.x || shape->position.y != newPos.y) {
        printf("FAIL: Shape position setting failed\n");
    }
    
    // Test rotation setting
    float rotation = 45.0f;
    SetShapeRotation(shape, rotation);
    
    if (shape->rotation != rotation) {
        printf("FAIL: Shape rotation setting failed\n");
    }
    
    // Test color setting
    Color newColor = PURPLE;
    SetShapeColor(shape, newColor);
    
    if (shape->color.r != newColor.r || shape->color.g != newColor.g || 
        shape->color.b != newColor.b || shape->color.a != newColor.a) {
        printf("FAIL: Shape color setting failed\n");
    }
    
    printf("PASS: Shape manipulation test completed\n");
    FreeShape(shape);
}

void test_animation() {
    printf("\nTesting animation...\n");
    
    // Create a shape and animation
    Vector2 position = {500, 500};
    RayPals2DShape* shape = CreateSquare(position, 30, ORANGE);
    
    RayPalsAnimation animation = {
        .isAnimated = true,
        .animationSpeed = 1.0f,
        .animationTime = 0.0f,
        .scaleMin = 0.5f,
        .scaleMax = 1.5f,
        .rotationSpeed = 90.0f,
        .colorStart = ORANGE,
        .colorEnd = YELLOW,
        .pingPong = true
    };
    
    // Test animation update
    float deltaTime = 0.016f; // ~60 FPS
    UpdateShapeAnimation(shape, &animation, deltaTime);
    
    // Verify animation properties
    if (shape->rotation == 0.0f) {
        printf("FAIL: Animation rotation not applied\n");
    }
    
    if (shape->color.r == animation.colorStart.r && 
        shape->color.g == animation.colorStart.g && 
        shape->color.b == animation.colorStart.b) {
        printf("FAIL: Animation color not applied\n");
    }
    
    printf("PASS: Animation test completed\n");
    FreeShape(shape);
}

void test_3d_robot_creation() {
    printf("\nTesting 3D robot creation...\n");
    
    // Create a 3D robot
    Vector3 position = {0.0f, 1.0f, 0.0f};
    float size = 2.0f;
    Color bodyColor = RED;
    Color detailColor = YELLOW;
    
    RayPals3DSprite* robot = Create3DRobot(position, size, bodyColor, detailColor);
    
    if (robot == NULL) {
        printf("FAIL: 3D robot creation failed\n");
        return;
    }
    
    // Verify robot properties
    if (robot->shapeCount != 6) {  // Robot should have 6 parts: body, head, 2 eyes, 2 arms
        printf("FAIL: 3D robot shape count incorrect (expected 6, got %d)\n", robot->shapeCount);
    }
    
    if (robot->position.x != position.x || 
        robot->position.y != position.y || 
        robot->position.z != position.z) {
        printf("FAIL: 3D robot position incorrect\n");
    }
    
    // Test robot body (first shape)
    RayPals3DShape* body = robot->shapes[0];
    if (body->type != RAYPALS_CUBE) {
        printf("FAIL: 3D robot body should be a cube\n");
    }
    
    if (body->color.r != bodyColor.r || 
        body->color.g != bodyColor.g || 
        body->color.b != bodyColor.b) {
        printf("FAIL: 3D robot body color incorrect\n");
    }
    
    // Test robot eyes (shapes 2 and 3)
    RayPals3DShape* leftEye = robot->shapes[2];
    RayPals3DShape* rightEye = robot->shapes[3];
    
    if (leftEye->type != RAYPALS_CYLINDER || rightEye->type != RAYPALS_CYLINDER) {
        printf("FAIL: 3D robot eyes should be cylinders\n");
    }
    
    if (leftEye->color.r != detailColor.r || 
        leftEye->color.g != detailColor.g || 
        leftEye->color.b != detailColor.b) {
        printf("FAIL: 3D robot eye color incorrect\n");
    }
    
    // Test rotation functions
    Vector3 rotation = {10.0f, 20.0f, 30.0f};
    Set3DSpriteRotation(robot, rotation);
    
    if (robot->rotation.x != rotation.x || 
        robot->rotation.y != rotation.y || 
        robot->rotation.z != rotation.z) {
        printf("FAIL: 3D robot rotation setting failed\n");
    }
    
    // Test position setting
    Vector3 newPos = {5.0f, 5.0f, 5.0f};
    Set3DSpritePosition(robot, newPos);
    
    if (robot->position.x != newPos.x || 
        robot->position.y != newPos.y || 
        robot->position.z != newPos.z) {
        printf("FAIL: 3D robot position setting failed\n");
    }
    
    printf("PASS: 3D robot creation test completed\n");
    Free3DSprite(robot);
} 