#include "raypals.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include "rlgl.h"

// ----------------------------------------------------------------------------
// Helper functions for drawing complex shapes
// ----------------------------------------------------------------------------

static void DrawPolygon(float radius, int sides, Color color) {
    if (sides < 3) sides = 3;
    float angleStep = 2.0f * PI / sides;
    
    rlBegin(RL_TRIANGLES);
        rlColor4ub(color.r, color.g, color.b, color.a);
        
        for (int i = 0; i < sides; i++) {
            float angle1 = i * angleStep;
            float angle2 = (i + 1) * angleStep;
            
            rlVertex2f(0, 0);
            rlVertex2f(cosf(angle1) * radius, sinf(angle1) * radius);
            rlVertex2f(cosf(angle2) * radius, sinf(angle2) * radius);
        }
    rlEnd();
}

static void DrawPolygonLines(float radius, int sides, Color color) {
    if (sides < 3) sides = 3;
    float angleStep = 2.0f * PI / sides;
    
    rlBegin(RL_LINES);
        rlColor4ub(color.r, color.g, color.b, color.a);
        
        for (int i = 0; i < sides; i++) {
            float angle1 = i * angleStep;
            float angle2 = (i + 1) * angleStep;
            
            rlVertex2f(cosf(angle1) * radius, sinf(angle1) * radius);
            rlVertex2f(cosf(angle2) * radius, sinf(angle2) * radius);
        }
    rlEnd();
}

static void DrawArrow(float x, float y, float size, float rotation, Color color) {
    // Draw the arrow shaft
    DrawRectangle(x - size/2, y - size/10, size, size/5, color);
    
    // Draw the arrow head
    Vector2 v1 = { x + size/2, y };
    Vector2 v2 = { x + size/4, y - size/4 };
    Vector2 v3 = { x + size/4, y + size/4 };
    
    DrawTriangle(v1, v2, v3, color);
}

static void DrawArrowLines(float x, float y, float size, float rotation, Color color) {
    // Draw the arrow shaft
    DrawRectangleLines(x - size/2, y - size/10, size, size/5, color);
    
    // Draw the arrow head
    Vector2 v1 = { x + size/2, y };
    Vector2 v2 = { x + size/4, y - size/4 };
    Vector2 v3 = { x + size/4, y + size/4 };
    
    DrawTriangleLines(v1, v2, v3, color);
}

static void DrawWaterDrop(float radius, Color color, bool filled) {
    // Draw circular part (top of drop)
    if (filled) {
        DrawCircle(0, -radius*0.3f, radius*0.7f, color);
    } else {
        DrawCircleLines(0, -radius*0.3f, radius*0.7f, color);
    }
    
    // Draw triangular part (bottom of drop)
    Vector2 v1 = { 0, radius*0.9f };
    Vector2 v2 = { -radius*0.7f, -radius*0.1f };
    Vector2 v3 = { radius*0.7f, -radius*0.1f };
    
    if (filled) {
        DrawTriangle(v1, v2, v3, color);
    } else {
        DrawTriangleLines(v1, v2, v3, color);
    }
}

// ----------------------------------------------------------------------------
// 2D Shape Functions
// ----------------------------------------------------------------------------

RayPals2DShape* CreateSquare(Vector2 position, float size, Color color) {
    RayPals2DShape* shape = (RayPals2DShape*)malloc(sizeof(RayPals2DShape));
    if (shape == NULL) return NULL;
    
    shape->type = RAYPALS_SQUARE;
    shape->position = position;
    shape->size = (Vector2){ size, size };
    shape->rotation = 0.0f;
    shape->color = color;
    shape->filled = true;
    shape->thickness = 2.0f;
    shape->segments = 0;
    shape->points = 0;
    shape->visible = true;
    
    return shape;
}

RayPals2DShape* CreateRectangle(Vector2 position, Vector2 size, Color color) {
    RayPals2DShape* shape = (RayPals2DShape*)malloc(sizeof(RayPals2DShape));
    if (shape == NULL) return NULL;
    
    shape->type = RAYPALS_RECTANGLE;
    shape->position = position;
    shape->size = size;
    shape->rotation = 0.0f;
    shape->color = color;
    shape->filled = true;
    shape->thickness = 2.0f;
    shape->segments = 0;
    shape->points = 0;
    shape->visible = true;
    
    return shape;
}

RayPals2DShape* CreateCircle(Vector2 position, float radius, Color color) {
    RayPals2DShape* shape = (RayPals2DShape*)malloc(sizeof(RayPals2DShape));
    if (shape == NULL) return NULL;
    
    shape->type = RAYPALS_CIRCLE;
    shape->position = position;
    shape->size = (Vector2){ radius * 2, radius * 2 };
    shape->rotation = 0.0f;
    shape->color = color;
    shape->filled = true;
    shape->thickness = 2.0f;
    shape->segments = 36;  // Default segment count
    shape->points = 0;
    shape->visible = true;
    
    return shape;
}

RayPals2DShape* CreateTriangle(Vector2 position, float size, Color color) {
    RayPals2DShape* shape = (RayPals2DShape*)malloc(sizeof(RayPals2DShape));
    if (shape == NULL) return NULL;
    
    shape->type = RAYPALS_TRIANGLE;
    shape->position = position;
    shape->size = (Vector2){ size, size };
    shape->rotation = 0.0f;
    shape->color = color;
    shape->filled = true;
    shape->thickness = 2.0f;
    shape->segments = 0;
    shape->points = 0;
    shape->visible = true;
    
    return shape;
}

RayPals2DShape* CreateStar(Vector2 position, float size, int points, Color color) {
    RayPals2DShape* shape = (RayPals2DShape*)malloc(sizeof(RayPals2DShape));
    if (shape == NULL) return NULL;
    
    // Default to 5 points if invalid value is provided
    if (points < 3) points = 5;
    
    // Use yellow as default if no color is specified
    if (color.r == 0 && color.g == 0 && color.b == 0 && color.a == 0) {
        color = YELLOW;
    }
    
    shape->type = RAYPALS_STAR;
    shape->position = position;
    shape->size = (Vector2){ size, size };
    shape->rotation = 0.0f;
    shape->color = color;
    shape->filled = true;
    shape->thickness = 2.0f;
    shape->segments = 0;
    shape->points = points;
    shape->visible = true;
    
    return shape;
}

RayPals2DShape* CreatePolygon(Vector2 position, float radius, int sides, Color color) {
    if (sides < 3) sides = 3; // Minimum 3 sides
    
    RayPals2DShape* shape = (RayPals2DShape*)malloc(sizeof(RayPals2DShape));
    if (shape == NULL) return NULL;
    
    shape->type = RAYPALS_POLYGON;
    shape->position = position;
    shape->size = (Vector2){ radius * 2, radius * 2 };
    shape->rotation = 0.0f;
    shape->color = color;
    shape->filled = true;
    shape->thickness = 2.0f;
    shape->segments = sides;
    shape->points = sides;
    shape->visible = true;
    
    return shape;
}

RayPals2DShape* CreateArrow(Vector2 position, float size, float direction, Color color) {
    RayPals2DShape* shape = (RayPals2DShape*)malloc(sizeof(RayPals2DShape));
    if (shape == NULL) return NULL;
    
    shape->type = RAYPALS_ARROW;
    shape->position = position;
    shape->size = (Vector2){ size, size };
    shape->rotation = direction;
    shape->color = color;
    shape->filled = true;
    shape->thickness = 2.0f;
    shape->segments = 0;
    shape->points = 0;
    shape->visible = true;
    
    return shape;
}

RayPals2DShape* CreateWaterDrop(Vector2 position, float size, float rotation, Color color) {
    RayPals2DShape* shape = (RayPals2DShape*)malloc(sizeof(RayPals2DShape));
    if (shape == NULL) return NULL;
    
    // Setup basic properties
    shape->type = RAYPALS_WATER_DROP; // Use water drop type for proper rendering
    shape->position = position;
    shape->size = (Vector2){ size, size * 1.5f }; // Water drop is taller than wide
    shape->rotation = rotation;
    shape->color = color;
    shape->filled = true;
    shape->thickness = 2.0f;
    shape->segments = 16; // Use enough segments for smooth drop shape
    shape->points = 0;
    shape->visible = true;
    
    return shape;
}

// ----------------------------------------------------------------------------
// 3D Shape Functions
// ----------------------------------------------------------------------------

RayPals3DShape* CreateCube(Vector3 position, Vector3 size, Color color) {
    RayPals3DShape* shape = (RayPals3DShape*)malloc(sizeof(RayPals3DShape));
    if (shape == NULL) return NULL;
    
    shape->type = RAYPALS_CUBE;
    shape->position = position;
    shape->size = size;
    shape->rotation = (Vector3){ 0.0f, 0.0f, 0.0f };
    shape->color = color;
    shape->wireframe = false;
    shape->thickness = 2.0f;
    shape->segments = 0;
    shape->visible = true;
    
    return shape;
}

RayPals3DShape* CreateSphere(Vector3 position, float radius, int segments, Color color) {
    RayPals3DShape* shape = (RayPals3DShape*)malloc(sizeof(RayPals3DShape));
    if (shape == NULL) return NULL;
    
    shape->type = RAYPALS_SPHERE;
    shape->position = position;
    shape->size = (Vector3){ radius * 2, radius * 2, radius * 2 };
    shape->rotation = (Vector3){ 0.0f, 0.0f, 0.0f };
    shape->color = color;
    shape->wireframe = false;
    shape->thickness = 2.0f;
    shape->segments = segments;
    shape->visible = true;
    
    return shape;
}

RayPals3DShape* CreateCone(Vector3 position, float radius, float height, int segments, Color color) {
    RayPals3DShape* shape = (RayPals3DShape*)malloc(sizeof(RayPals3DShape));
    if (shape == NULL) return NULL;
    
    shape->type = RAYPALS_CONE;
    shape->position = position;
    shape->size = (Vector3){ radius * 2, height, radius * 2 };
    shape->rotation = (Vector3){ 0.0f, 0.0f, 0.0f };
    shape->color = color;
    shape->wireframe = false;
    shape->thickness = 2.0f;
    shape->segments = segments;
    shape->visible = true;
    
    return shape;
}

RayPals3DShape* CreateCylinder(Vector3 position, float radius, float height, int segments, Color color) {
    RayPals3DShape* shape = (RayPals3DShape*)malloc(sizeof(RayPals3DShape));
    if (shape == NULL) return NULL;
    
    shape->type = RAYPALS_CYLINDER;
    shape->position = position;
    shape->size = (Vector3){ radius * 2, height, radius * 2 };
    shape->rotation = (Vector3){ 0.0f, 0.0f, 0.0f };
    shape->color = color;
    shape->wireframe = false;
    shape->thickness = 2.0f;
    shape->segments = segments;
    shape->visible = true;
    
    return shape;
}

// ----------------------------------------------------------------------------
// Drawing Functions
// ----------------------------------------------------------------------------

void Draw2DShape(RayPals2DShape* shape) {
    if (!shape || !shape->visible) return;
    
    // Save current matrix to restore later
    rlPushMatrix();
    
    // Translate to position and apply rotation
    rlTranslatef(shape->position.x, shape->position.y, 0.0f);
    rlRotatef(shape->rotation, 0.0f, 0.0f, 1.0f);
    
    switch (shape->type) {
        case RAYPALS_SQUARE:
        case RAYPALS_RECTANGLE: {
            float x = -shape->size.x/2;
            float y = -shape->size.y/2;
            
            if (shape->filled) {
                DrawRectangle(x, y, shape->size.x, shape->size.y, shape->color);
            } else {
                DrawRectangleLines(x, y, shape->size.x, shape->size.y, shape->color);
            }
        } break;
        
        case RAYPALS_CIRCLE: {
            float radius = shape->size.x/2;
            
            if (shape->filled) {
                DrawCircle(0, 0, radius, shape->color);
            } else {
                DrawCircleLines(0, 0, radius, shape->color);
            }
        } break;
        
        case RAYPALS_TRIANGLE: {
            float size = shape->size.x;
            Vector2 v1 = { 0, -size/2 };
            Vector2 v2 = { -size/2, size/2 };
            Vector2 v3 = { size/2, size/2 };
            
            if (shape->filled) {
                DrawTriangle(v1, v2, v3, shape->color);
            } else {
                DrawTriangleLines(v1, v2, v3, shape->color);
            }
        } break;
        
        case RAYPALS_STAR: {
            float outerRadius = shape->size.x/2;
            float innerRadius = outerRadius/3;  // Changed from outerRadius/2 to outerRadius/3 for more pronounced points
            int points = shape->points > 0 ? shape->points : 5;
            
            // Draw star using DrawPoly which is more reliable
            Vector2 starCenter = { 0, 0 };
            Vector2 starPoints[20]; // Maximum 10 points (2 vertices per point)
            int vertexCount = points * 2;
            
            if (vertexCount > 20) vertexCount = 20; // Safety check
            
            // Create vertices for star shape
            for (int i = 0; i < points * 2; i++) {
                float radius = i % 2 == 0 ? outerRadius : innerRadius;
                float angle = i * PI / points - PI/2;
                starPoints[i].x = cosf(angle) * radius;
                starPoints[i].y = sinf(angle) * radius;
            }
            
            if (shape->filled) {
                // Draw filled star polygon
                DrawPoly(starCenter, points * 2, outerRadius, shape->rotation, shape->color);
                
                // Draw explicit triangles as a fallback/supplement
                Vector2 center = { 0, 0 };
                for (int i = 0; i < points * 2; i+=2) {
                    int next = (i + 1) % (points * 2);
                    int nextOuter = (i + 2) % (points * 2);
                    DrawTriangle(center, starPoints[i], starPoints[next], shape->color);
                    DrawTriangle(center, starPoints[next], starPoints[nextOuter], shape->color);
                }
            } else {
                // Draw star outline
                for (int i = 0; i < points * 2; i++) {
                    int next = (i + 1) % (points * 2);
                    DrawLineV(starPoints[i], starPoints[next], shape->color);
                }
            }
        } break;
        
        case RAYPALS_POLYGON: {
            float radius = shape->size.x/2;
            
            if (shape->filled) {
                DrawPolygon(radius, shape->segments, shape->color);
            } else {
                DrawPolygonLines(radius, shape->segments, shape->color);
            }
        } break;
        
        case RAYPALS_ARROW: {
            float size = shape->size.x;
            
            if (shape->filled) {
                DrawArrow(0, 0, size, shape->rotation, shape->color);
            } else {
                DrawArrowLines(0, 0, size, shape->rotation, shape->color);
            }
        } break;
        
        case RAYPALS_WATER_DROP: {
            float radius = shape->size.x/2;
            DrawWaterDrop(radius, shape->color, shape->filled);
        } break;
        
        case RAYPALS_SKELETON: {
            // We'll draw a simple skeleton outline
            // The actual drawing is a simplified stick figure
            float width = shape->size.x;
            float height = shape->size.y;
            
            // Just draw a skeleton icon (simplified X shape with a circle on top)
            if (shape->filled) {
                DrawCircle(0, -height*0.35f, width*0.15f, shape->color); // Skull
                DrawLine(0, -height*0.2f, 0, height*0.2f, shape->color); // Spine
                DrawLine(-width*0.25f, -height*0.1f, width*0.25f, -height*0.1f, shape->color); // Shoulders
                DrawLine(-width*0.25f, height*0.2f, width*0.25f, height*0.2f, shape->color); // Hips
                DrawLine(-width*0.25f, -height*0.1f, -width*0.4f, 0, shape->color); // Left arm
                DrawLine(width*0.25f, -height*0.1f, width*0.4f, 0, shape->color); // Right arm
                DrawLine(0, height*0.2f, -width*0.3f, height*0.4f, shape->color); // Left leg
                DrawLine(0, height*0.2f, width*0.3f, height*0.4f, shape->color); // Right leg
            } else {
                float thickness = shape->thickness;
                DrawCircleLines(0, -height*0.35f, width*0.15f, shape->color); // Skull
                DrawLineEx((Vector2){0, -height*0.2f}, (Vector2){0, height*0.2f}, thickness, shape->color); // Spine
                DrawLineEx((Vector2){-width*0.25f, -height*0.1f}, (Vector2){width*0.25f, -height*0.1f}, thickness, shape->color); // Shoulders
                DrawLineEx((Vector2){-width*0.25f, height*0.2f}, (Vector2){width*0.25f, height*0.2f}, thickness, shape->color); // Hips
                DrawLineEx((Vector2){-width*0.25f, -height*0.1f}, (Vector2){-width*0.4f, 0}, thickness, shape->color); // Left arm
                DrawLineEx((Vector2){width*0.25f, -height*0.1f}, (Vector2){width*0.4f, 0}, thickness, shape->color); // Right arm
                DrawLineEx((Vector2){0, height*0.2f}, (Vector2){-width*0.3f, height*0.4f}, thickness, shape->color); // Left leg
                DrawLineEx((Vector2){0, height*0.2f}, (Vector2){width*0.3f, height*0.4f}, thickness, shape->color); // Right leg
            }
        } break;
        
        default: break;
    }
    
    // Restore matrix
    rlPopMatrix();
}

// Draw a 3D shape
// If camera is NULL, assumes transformations are already set (e.g., called from Draw3DSprite)
// If camera is provided, wraps drawing in BeginMode3D/EndMode3D
void Draw3DShape(RayPals3DShape* shape, Camera *camera) { // Changed to Camera pointer
    if (!shape || !shape->visible) return;

    bool calledDirectly = (camera != NULL);

    if (calledDirectly) {
        BeginMode3D(*camera);
    }

    rlPushMatrix();
    
    // Apply shape transformations (relative to current matrix state)
    rlTranslatef(shape->position.x, shape->position.y, shape->position.z);
    
    // Apply rotations in order: Y (yaw), X (pitch), Z (roll)
    rlRotatef(shape->rotation.y, 0.0f, 1.0f, 0.0f);
    rlRotatef(shape->rotation.x, 1.0f, 0.0f, 0.0f);
    rlRotatef(shape->rotation.z, 0.0f, 0.0f, 1.0f);
    
    switch (shape->type) {
        case RAYPALS_CUBE:
            if (shape->wireframe)
                DrawCubeWiresV((Vector3){ 0.0f, 0.0f, 0.0f }, shape->size, shape->color); // Use literal Vector3 zero
            else
                DrawCubeV((Vector3){ 0.0f, 0.0f, 0.0f }, shape->size, shape->color); // Use literal Vector3 zero
            break;
        case RAYPALS_SPHERE:
            if (shape->wireframe)
                DrawSphereWires((Vector3){ 0.0f, 0.0f, 0.0f }, shape->size.x / 2.0f, shape->segments, shape->segments, shape->color); // Use literal Vector3 zero
            else
                DrawSphereEx((Vector3){ 0.0f, 0.0f, 0.0f }, shape->size.x / 2.0f, shape->segments, shape->segments, shape->color); // Use literal Vector3 zero
            break;
        case RAYPALS_CONE:
            // Draw cone using DrawCylinder with top radius 0
            if (shape->wireframe)
                DrawCylinderWires((Vector3){ 0.0f, 0.0f, 0.0f }, 0.0f, shape->size.x / 2.0f, shape->size.y, shape->segments, shape->color); // Use DrawCylinderWires
            else
                DrawCylinder((Vector3){ 0.0f, 0.0f, 0.0f }, 0.0f, shape->size.x / 2.0f, shape->size.y, shape->segments, shape->color); // Use DrawCylinder
            break;
        case RAYPALS_CYLINDER:
            if (shape->wireframe)
                DrawCylinderWires((Vector3){ 0.0f, 0.0f, 0.0f }, shape->size.x / 2.0f, shape->size.x / 2.0f, shape->size.y, shape->segments, shape->color); // Use literal Vector3 zero
            else
                DrawCylinder((Vector3){ 0.0f, 0.0f, 0.0f }, shape->size.x / 2.0f, shape->size.x / 2.0f, shape->size.y, shape->segments, shape->color); // Use literal Vector3 zero
            break;
        default:
            // Unknown or unsupported 3D shape type
            break;
    }

    rlPopMatrix();

    if (calledDirectly) {
        EndMode3D();
    }
}

// ----------------------------------------------------------------------------
// Utility Functions
// ----------------------------------------------------------------------------

void SetShapeColor(RayPals2DShape* shape, Color color) {
    if (shape) shape->color = color;
}

void Set3DShapeColor(RayPals3DShape* shape, Color color) {
    if (shape) shape->color = color;
}

void SetShapeRotation(RayPals2DShape* shape, float rotation) {
    if (shape) shape->rotation = rotation;
}

void Set3DShapeRotation(RayPals3DShape* shape, Vector3 rotation) {
    if (shape) shape->rotation = rotation;
}

void SetShapePosition(RayPals2DShape* shape, Vector2 position) {
    if (shape) shape->position = position;
}

void Set3DShapePosition(RayPals3DShape* shape, Vector3 position) {
    if (shape) shape->position = position;
}

void RotateShape(RayPals2DShape* shape, float deltaTime, float speed) {
    if (!shape) return;
    
    // Update rotation based on speed and delta time
    shape->rotation += speed * deltaTime;
    
    // Normalize rotation to 0-360 degrees
    while (shape->rotation >= 360.0f) shape->rotation -= 360.0f;
    while (shape->rotation < 0.0f) shape->rotation += 360.0f;
}

void Rotate3DShape(RayPals3DShape* shape, float deltaTime, Vector3 speed) {
    if (!shape) return;
    
    // Update rotation based on speed and delta time
    shape->rotation.x += speed.x * deltaTime;
    shape->rotation.y += speed.y * deltaTime;
    shape->rotation.z += speed.z * deltaTime;
    
    // Normalize rotation to 0-360 degrees
    while (shape->rotation.x >= 360.0f) shape->rotation.x -= 360.0f;
    while (shape->rotation.x < 0.0f) shape->rotation.x += 360.0f;
    
    while (shape->rotation.y >= 360.0f) shape->rotation.y -= 360.0f;
    while (shape->rotation.y < 0.0f) shape->rotation.y += 360.0f;
    
    while (shape->rotation.z >= 360.0f) shape->rotation.z -= 360.0f;
    while (shape->rotation.z < 0.0f) shape->rotation.z += 360.0f;
}

void UpdateShapeAnimation(RayPals2DShape* shape, RayPalsAnimation* animation, float deltaTime) {
    if (!shape || !animation || !animation->isAnimated) return;
    
    // Update animation time
    animation->animationTime += deltaTime * animation->animationSpeed;
    
    // Calculate animation factor (0.0 to 1.0)
    float factor = sinf(animation->animationTime);
    if (animation->pingPong) {
        factor = (factor + 1.0f) / 2.0f;  // Convert to 0.0 - 1.0 range
    } else {
        // For non-pingpong animations, use absolute value to create a more pronounced effect
        factor = fabsf(factor);
    }
    
    // Apply rotation if rotation speed is not zero
    if (animation->rotationSpeed != 0) {
        shape->rotation += animation->rotationSpeed * deltaTime;
        
        // Normalize rotation to 0-360 degrees
        while (shape->rotation >= 360.0f) shape->rotation -= 360.0f;
        while (shape->rotation < 0.0f) shape->rotation += 360.0f;
    }
    
    // Apply scale if min/max are different
    if (animation->scaleMin != animation->scaleMax) {
        // Store the original size if not already stored
        if (animation->originalWidth == 0 && animation->originalHeight == 0) {
            animation->originalWidth = shape->size.x;
            animation->originalHeight = shape->size.y;
        }
        
        float scale = animation->scaleMin + factor * (animation->scaleMax - animation->scaleMin);
        
        // Apply scale to the original size, not the current size
        shape->size.x = animation->originalWidth * scale;
        shape->size.y = animation->originalHeight * scale;
    }
    
    // Apply color transition if colors are different
    if (!ColorIsEqual(animation->colorStart, animation->colorEnd)) {
        shape->color.r = (unsigned char)(animation->colorStart.r + factor * (animation->colorEnd.r - animation->colorStart.r));
        shape->color.g = (unsigned char)(animation->colorStart.g + factor * (animation->colorEnd.g - animation->colorStart.g));
        shape->color.b = (unsigned char)(animation->colorStart.b + factor * (animation->colorEnd.b - animation->colorStart.b));
        shape->color.a = (unsigned char)(animation->colorStart.a + factor * (animation->colorEnd.a - animation->colorStart.a));
    }
}

void Update3DShapeAnimation(RayPals3DShape* shape, RayPalsAnimation* animation, float deltaTime) {
    if (!shape || !animation || !animation->isAnimated) return;
    
    // Update animation time
    animation->animationTime += deltaTime * animation->animationSpeed;
    
    // Calculate animation factor (0.0 to 1.0)
    float factor = sinf(animation->animationTime);
    if (animation->pingPong) {
        factor = (factor + 1.0f) / 2.0f;  // Convert to 0.0 - 1.0 range
    } else {
        // For non-pingpong animations, use absolute value for more pronounced effect
        factor = fabsf(factor);
    }
    
    // Apply rotation if rotation speed is not zero
    if (animation->rotationSpeed != 0) {
        shape->rotation.y += animation->rotationSpeed * deltaTime;
        
        // Normalize rotation
        while (shape->rotation.y >= 360.0f) shape->rotation.y -= 360.0f;
        while (shape->rotation.y < 0.0f) shape->rotation.y += 360.0f;
    }
    
    // Apply scale if min/max are different
    if (animation->scaleMin != animation->scaleMax) {
        // Store the original size if not already stored
        if (animation->originalWidth == 0) {
            animation->originalWidth = shape->size.x;
            animation->originalHeight = shape->size.y;
            animation->originalDepth = shape->size.z;
        }
        
        float scale = animation->scaleMin + factor * (animation->scaleMax - animation->scaleMin);
        
        // Apply scale to the original size, not the current size
        shape->size.x = animation->originalWidth * scale;
        shape->size.y = animation->originalHeight * scale;
        shape->size.z = animation->originalDepth * scale;
    }
    
    // Apply color transition if colors are different
    if (!ColorIsEqual(animation->colorStart, animation->colorEnd)) {
        shape->color.r = (unsigned char)(animation->colorStart.r + factor * (animation->colorEnd.r - animation->colorStart.r));
        shape->color.g = (unsigned char)(animation->colorStart.g + factor * (animation->colorEnd.g - animation->colorStart.g));
        shape->color.b = (unsigned char)(animation->colorStart.b + factor * (animation->colorEnd.b - animation->colorStart.b));
        shape->color.a = (unsigned char)(animation->colorStart.a + factor * (animation->colorEnd.a - animation->colorStart.a));
    }
}

void FreeShape(RayPals2DShape* shape) {
    if (shape) {
        free(shape);
    }
}

void Free3DShape(RayPals3DShape* shape) {
    if (shape) {
        free(shape);
    }
}

// ----------------------------------------------------------------------------
// Sprite Functions
// ----------------------------------------------------------------------------

RayPalsSprite* CreateSprite(int initialCapacity) {
    RayPalsSprite* sprite = (RayPalsSprite*)malloc(sizeof(RayPalsSprite));
    if (sprite == NULL) return NULL;
    
    sprite->shapes = (RayPals2DShape**)malloc(sizeof(RayPals2DShape*) * initialCapacity);
    if (sprite->shapes == NULL) {
        free(sprite);
        return NULL;
    }
    
    sprite->shapeCount = 0;
    sprite->position = (Vector2){ 0, 0 };
    sprite->rotation = 0.0f;
    sprite->scale = 1.0f;
    sprite->visible = true;
    
    return sprite;
}

void AddShapeToSprite(RayPalsSprite* sprite, RayPals2DShape* shape) {
    if (!sprite || !shape) return;
    
    // Resize the array if needed (simple implementation, not optimized)
    RayPals2DShape** newShapes = (RayPals2DShape**)realloc(sprite->shapes, 
                                  sizeof(RayPals2DShape*) * (sprite->shapeCount + 1));
    if (newShapes == NULL) return;
    
    sprite->shapes = newShapes;
    sprite->shapes[sprite->shapeCount] = shape;
    sprite->shapeCount++;
}

RayPalsSprite* CreateSimpleCharacter(Vector2 position, float size, Color bodyColor, Color headColor) {
    RayPalsSprite* sprite = CreateSprite(2);
    if (sprite == NULL) return NULL;
    
    // Create body (circle)
    RayPals2DShape* body = CreateCircle((Vector2){ 0, size/4 }, size/2, bodyColor);
    if (body == NULL) {
        FreeSprite(sprite);
        return NULL;
    }
    
    // Create head (circle)
    RayPals2DShape* head = CreateCircle((Vector2){ 0, -size/2 }, size/3, headColor);
    if (head == NULL) {
        FreeShape(body);
        FreeSprite(sprite);
        return NULL;
    }
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, body);
    AddShapeToSprite(sprite, head);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateSimpleTree(Vector2 position, float size, Color trunkColor, Color leafColor) {
    RayPalsSprite* sprite = CreateSprite(2);
    if (sprite == NULL) return NULL;
    
    // Create trunk (rectangle)
    RayPals2DShape* trunk = CreateRectangle((Vector2){ 0, size/4 }, (Vector2){ size/5, size/2 }, trunkColor);
    if (trunk == NULL) {
        FreeSprite(sprite);
        return NULL;
    }
    
    // Create leaves (triangle)
    RayPals2DShape* leaves = CreateTriangle((Vector2){ 0, -size/3 }, size, leafColor);
    if (leaves == NULL) {
        FreeShape(trunk);
        FreeSprite(sprite);
        return NULL;
    }
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, trunk);
    AddShapeToSprite(sprite, leaves);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateCloud(Vector2 position, float size, Color color) {
    RayPalsSprite* sprite = CreateSprite(4);
    if (sprite == NULL) return NULL;
    
    // Create cloud circles
    RayPals2DShape* cloud1 = CreateCircle((Vector2){ -size/3, 0 }, size/3, color);
    RayPals2DShape* cloud2 = CreateCircle((Vector2){ 0, -size/4 }, size/2.5f, color);
    RayPals2DShape* cloud3 = CreateCircle((Vector2){ size/3, 0 }, size/3, color);
    RayPals2DShape* cloud4 = CreateCircle((Vector2){ 0, size/8 }, size/3, color);
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, cloud1);
    AddShapeToSprite(sprite, cloud2);
    AddShapeToSprite(sprite, cloud3);
    AddShapeToSprite(sprite, cloud4);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateHouse(Vector2 position, float size, Color wallColor, Color roofColor) {
    RayPalsSprite* sprite = CreateSprite(5);
    if (sprite == NULL) return NULL;
    
    // House base
    RayPals2DShape* base = CreateRectangle((Vector2){ 0, size/8 }, (Vector2){ size, size*0.8f }, wallColor);
    
    // Roof
    RayPals2DShape* roof = CreateTriangle((Vector2){ 0, -size/2 }, size*1.2f, roofColor);
    
    // Door
    RayPals2DShape* door = CreateRectangle((Vector2){ 0, size/3 }, (Vector2){ size/4, size/3 }, BROWN);
    
    // Windows
    RayPals2DShape* window1 = CreateRectangle((Vector2){ -size/3, 0 }, (Vector2){ size/5, size/5 }, SKYBLUE);
    RayPals2DShape* window2 = CreateRectangle((Vector2){ size/3, 0 }, (Vector2){ size/5, size/5 }, SKYBLUE);
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, base);
    AddShapeToSprite(sprite, roof);
    AddShapeToSprite(sprite, door);
    AddShapeToSprite(sprite, window1);
    AddShapeToSprite(sprite, window2);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateCastle(Vector2 position, float size, Color wallColor, Color roofColor) {
    RayPalsSprite* sprite = CreateSprite(8);
    if (sprite == NULL) return NULL;
    
    // Main castle body (rectangle)
    RayPals2DShape* body = CreateRectangle((Vector2){ 0, size/4 }, (Vector2){ size, size*0.8f }, wallColor);
    
    // Main tower (rectangle)
    RayPals2DShape* mainTower = CreateRectangle((Vector2){ 0, -size/3 }, (Vector2){ size*0.4f, size*0.6f }, wallColor);
    
    // Side towers (rectangles)
    RayPals2DShape* leftTower = CreateRectangle((Vector2){ -size*0.4f, -size/3 }, (Vector2){ size*0.3f, size*0.5f }, wallColor);
    RayPals2DShape* rightTower = CreateRectangle((Vector2){ size*0.4f, -size/3 }, (Vector2){ size*0.3f, size*0.5f }, wallColor);
    
    // Roofs (triangles)
    RayPals2DShape* mainRoof = CreateTriangle((Vector2){ 0, -size*0.6f }, size*0.5f, roofColor);
    RayPals2DShape* leftRoof = CreateTriangle((Vector2){ -size*0.4f, -size*0.55f }, size*0.4f, roofColor);
    RayPals2DShape* rightRoof = CreateTriangle((Vector2){ size*0.4f, -size*0.55f }, size*0.4f, roofColor);
    
    // Castle gate (rectangle)
    RayPals2DShape* gate = CreateRectangle((Vector2){ 0, size/3 }, (Vector2){ size*0.3f, size*0.4f }, BROWN);
    
    // Windows (circles)
    RayPals2DShape* window1 = CreateCircle((Vector2){ -size*0.2f, 0 }, size*0.1f, SKYBLUE);
    RayPals2DShape* window2 = CreateCircle((Vector2){ size*0.2f, 0 }, size*0.1f, SKYBLUE);
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, body);
    AddShapeToSprite(sprite, mainTower);
    AddShapeToSprite(sprite, leftTower);
    AddShapeToSprite(sprite, rightTower);
    AddShapeToSprite(sprite, mainRoof);
    AddShapeToSprite(sprite, leftRoof);
    AddShapeToSprite(sprite, rightRoof);
    AddShapeToSprite(sprite, gate);
    AddShapeToSprite(sprite, window1);
    AddShapeToSprite(sprite, window2);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateBush(Vector2 position, float size, Color color) {
    RayPalsSprite* sprite = CreateSprite(3);
    if (sprite == NULL) return NULL;
    
    // Bush circles
    RayPals2DShape* bush1 = CreateCircle((Vector2){ -size/4, 0 }, size/2, color);
    RayPals2DShape* bush2 = CreateCircle((Vector2){ size/4, 0 }, size/2, color);
    RayPals2DShape* bush3 = CreateCircle((Vector2){ 0, -size/4 }, size/2, color);
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, bush1);
    AddShapeToSprite(sprite, bush2);
    AddShapeToSprite(sprite, bush3);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateRock(Vector2 position, float size, Color color) {
    RayPalsSprite* sprite = CreateSprite(1);
    if (sprite == NULL) return NULL;
    
    // Just create a simple circle for the rock with the specified color
    RayPals2DShape* rock = CreateCircle((Vector2){ 0, 0 }, size/2, color);
    
    // Add shape to sprite
    AddShapeToSprite(sprite, rock);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateRobotCharacter(Vector2 position, float size, Color primaryColor, Color secondaryColor) {
    RayPalsSprite* sprite = CreateSprite(4);
    if (sprite == NULL) return NULL;
    
    // Create robot parts
    RayPals2DShape* body = CreateRectangle((Vector2){ 0, 0 }, (Vector2){ size*0.8f, size }, primaryColor);
    RayPals2DShape* head = CreateRectangle((Vector2){ 0, -size*0.6f }, (Vector2){ size*0.6f, size*0.5f }, primaryColor);
    
    // Eyes
    RayPals2DShape* eye1 = CreateCircle((Vector2){ -size*0.15f, -size*0.6f }, size*0.1f, secondaryColor);
    RayPals2DShape* eye2 = CreateCircle((Vector2){ size*0.15f, -size*0.6f }, size*0.1f, secondaryColor);
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, body);
    AddShapeToSprite(sprite, head);
    AddShapeToSprite(sprite, eye1);
    AddShapeToSprite(sprite, eye2);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateAnimalCharacter(Vector2 position, float size, Color bodyColor, Color detailColor) {
    RayPalsSprite* sprite = CreateSprite(5);
    if (sprite == NULL) return NULL;
    
    // Create animal parts (e.g., a simple cat)
    RayPals2DShape* body = CreateCircle((Vector2){ 0, size*0.1f }, size*0.4f, bodyColor);
    RayPals2DShape* head = CreateCircle((Vector2){ 0, -size*0.3f }, size*0.3f, bodyColor);
    RayPals2DShape* ear1 = CreateTriangle((Vector2){ -size*0.2f, -size*0.5f }, size*0.2f, bodyColor);
    RayPals2DShape* ear2 = CreateTriangle((Vector2){ size*0.2f, -size*0.5f }, size*0.2f, bodyColor);
    
    // Eyes
    RayPals2DShape* eye1 = CreateCircle((Vector2){ -size*0.1f, -size*0.3f }, size*0.05f, detailColor);
    RayPals2DShape* eye2 = CreateCircle((Vector2){ size*0.1f, -size*0.3f }, size*0.05f, detailColor);
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, body);
    AddShapeToSprite(sprite, head);
    AddShapeToSprite(sprite, ear1);
    AddShapeToSprite(sprite, ear2);
    AddShapeToSprite(sprite, eye1);
    AddShapeToSprite(sprite, eye2);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateGhost(Vector2 position, float size, Color color) {
    RayPalsSprite* sprite = CreateSprite(4);
    if (sprite == NULL) return NULL;
    
    // Main ghost body (half-circle on top, wavy bottom)
    RayPals2DShape* body = CreateCircle((Vector2){ 0, -size*0.15f }, size*0.4f, color);
    body->filled = true;
    
    // Bottom part of ghost created with triangles
    RayPals2DShape* bottom1 = CreateTriangle((Vector2){ -size*0.3f, size*0.3f }, size*0.3f, color);
    RayPals2DShape* bottom2 = CreateTriangle((Vector2){ 0, size*0.3f }, size*0.3f, color);
    RayPals2DShape* bottom3 = CreateTriangle((Vector2){ size*0.3f, size*0.3f }, size*0.3f, color);
    
    // Eyes
    RayPals2DShape* eye1 = CreateCircle((Vector2){ -size*0.15f, -size*0.2f }, size*0.07f, BLACK);
    RayPals2DShape* eye2 = CreateCircle((Vector2){ size*0.15f, -size*0.2f }, size*0.07f, BLACK);
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, body);
    AddShapeToSprite(sprite, bottom1);
    AddShapeToSprite(sprite, bottom2);
    AddShapeToSprite(sprite, bottom3);
    AddShapeToSprite(sprite, eye1);
    AddShapeToSprite(sprite, eye2);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateButton(Vector2 position, Vector2 size, Color bgColor, Color borderColor) {
    RayPalsSprite* sprite = CreateSprite(2);
    if (sprite == NULL) return NULL;
    
    // Button background
    RayPals2DShape* background = CreateRectangle((Vector2){ 0, 0 }, size, bgColor);
    background->filled = true;
    
    // Button border
    RayPals2DShape* border = CreateRectangle((Vector2){ 0, 0 }, size, borderColor);
    border->filled = false;
    border->thickness = 2.0f;
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, background);
    AddShapeToSprite(sprite, border);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateHealthBar(Vector2 position, float width, float fillPercent, Color emptyColor, Color fillColor) {
    if (fillPercent < 0.0f) fillPercent = 0.0f;
    if (fillPercent > 1.0f) fillPercent = 1.0f;
    
    RayPalsSprite* sprite = CreateSprite(2);
    if (sprite == NULL) return NULL;
    
    float height = width * 0.15f;
    
    // Bar background
    RayPals2DShape* background = CreateRectangle((Vector2){ 0, 0 }, (Vector2){ width, height }, emptyColor);
    background->filled = true;
    
    // Calculate fill width
    float fillWidth = width * fillPercent;
    float fillOffsetX = (width - fillWidth) * 0.5f;
    
    // Bar fill
    RayPals2DShape* fill = CreateRectangle((Vector2){ -fillOffsetX, 0 }, (Vector2){ fillWidth, height * 0.8f }, fillColor);
    fill->filled = true;
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, background);
    AddShapeToSprite(sprite, fill);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

void DrawSprite(RayPalsSprite* sprite) {
    if (!sprite || !sprite->visible) return;
    
    // Save current matrix to restore later
    rlPushMatrix();
    
    // Apply sprite transformations
    rlTranslatef(sprite->position.x, sprite->position.y, 0.0f);
    rlRotatef(sprite->rotation, 0.0f, 0.0f, 1.0f);
    rlScalef(sprite->scale, sprite->scale, 1.0f);
    
    // Draw all shapes in the sprite
    for (int i = 0; i < sprite->shapeCount; i++) {
        Draw2DShape(sprite->shapes[i]);
    }
    
    // Restore matrix
    rlPopMatrix();
}

void RotateSprite(RayPalsSprite* sprite, float deltaTime, float speed) {
    if (!sprite) return;
    
    sprite->rotation += speed * deltaTime;
    
    // Normalize rotation to 0-360 degrees
    while (sprite->rotation >= 360.0f) sprite->rotation -= 360.0f;
    while (sprite->rotation < 0.0f) sprite->rotation += 360.0f;
}

void SetSpritePosition(RayPalsSprite* sprite, Vector2 position) {
    if (sprite) sprite->position = position;
}

void SetSpriteRotation(RayPalsSprite* sprite, float rotation) {
    if (sprite) sprite->rotation = rotation;
}

void SetSpriteScale(RayPalsSprite* sprite, float scale) {
    if (sprite) sprite->scale = scale;
}

void FreeSprite(RayPalsSprite* sprite) {
    if (!sprite) return;
    
    // Free all shapes in the sprite
    for (int i = 0; i < sprite->shapeCount; i++) {
        FreeShape(sprite->shapes[i]);
    }
    
    // Free the shapes array and the sprite itself
    free(sprite->shapes);
    free(sprite);
}

RayPalsSprite* CreateCar(Vector2 position, float size, Color bodyColor, Color detailColor) {
    RayPalsSprite* sprite = CreateSprite(5);
    if (sprite == NULL) return NULL;
    
    // Car body (main rectangle)
    RayPals2DShape* body = CreateRectangle((Vector2){ 0, 0 }, (Vector2){ size, size*0.5f }, bodyColor);
    
    // Car roof/cabin (smaller rectangle)
    RayPals2DShape* roof = CreateRectangle((Vector2){ -size*0.1f, -size*0.15f }, (Vector2){ size*0.5f, size*0.3f }, bodyColor);
    
    // Wheels
    RayPals2DShape* wheel1 = CreateCircle((Vector2){ -size*0.3f, size*0.25f }, size*0.15f, detailColor);
    RayPals2DShape* wheel2 = CreateCircle((Vector2){ size*0.3f, size*0.25f }, size*0.15f, detailColor);
    
    // Windshield
    RayPals2DShape* windshield = CreateRectangle((Vector2){ -size*0.1f, -size*0.15f }, (Vector2){ size*0.5f*0.8f, size*0.3f*0.8f }, SKYBLUE);
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, body);
    AddShapeToSprite(sprite, roof);
    AddShapeToSprite(sprite, wheel1);
    AddShapeToSprite(sprite, wheel2);
    AddShapeToSprite(sprite, windshield);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateTank(Vector2 position, float size, Color bodyColor, Color detailColor) {
    RayPalsSprite* sprite = CreateSprite(4);
    if (sprite == NULL) return NULL;
    
    // Tank base/body
    RayPals2DShape* base = CreateRectangle((Vector2){ 0, 0 }, (Vector2){ size*1.2f, size*0.5f }, bodyColor);
    
    // Tank turret (circle)
    RayPals2DShape* turret = CreateCircle((Vector2){ 0, -size*0.2f }, size*0.25f, bodyColor);
    
    // Tank cannon (rectangle)
    RayPals2DShape* cannon = CreateRectangle((Vector2){ size*0.4f, -size*0.2f }, (Vector2){ size*0.6f, size*0.1f }, bodyColor);
    
    // Tank treads
    RayPals2DShape* treads = CreateRectangle((Vector2){ 0, size*0.15f }, (Vector2){ size*1.1f, size*0.2f }, detailColor);
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, treads);
    AddShapeToSprite(sprite, base);
    AddShapeToSprite(sprite, turret);
    AddShapeToSprite(sprite, cannon);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateMotorcycle(Vector2 position, float size, Color bodyColor, Color detailColor) {
    RayPalsSprite* sprite = CreateSprite(5);
    if (sprite == NULL) return NULL;
    
    // Wheels
    RayPals2DShape* wheel1 = CreateCircle((Vector2){ -size*0.3f, size*0.1f }, size*0.2f, detailColor);
    RayPals2DShape* wheel2 = CreateCircle((Vector2){ size*0.3f, size*0.1f }, size*0.2f, detailColor);
    
    // Motorcycle body
    RayPals2DShape* body = CreateRectangle((Vector2){ 0, -size*0.1f }, (Vector2){ size*0.8f, size*0.15f }, bodyColor);
    
    // Handlebars
    RayPals2DShape* handlebar = CreateRectangle((Vector2){ size*0.3f, -size*0.2f }, (Vector2){ size*0.1f, size*0.2f }, bodyColor);
    
    // Seat
    RayPals2DShape* seat = CreateRectangle((Vector2){ -size*0.1f, -size*0.2f }, (Vector2){ size*0.3f, size*0.1f }, BLACK);
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, wheel1);
    AddShapeToSprite(sprite, wheel2);
    AddShapeToSprite(sprite, body);
    AddShapeToSprite(sprite, handlebar);
    AddShapeToSprite(sprite, seat);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateSkateboard(Vector2 position, float size, Color deckColor, Color wheelColor) {
    RayPalsSprite* sprite = CreateSprite(3);
    if (sprite == NULL) return NULL;
    
    // Deck (board)
    RayPals2DShape* deck = CreateRectangle((Vector2){ 0, 0 }, (Vector2){ size, size*0.25f }, deckColor);
    
    // Wheels
    RayPals2DShape* frontWheels = CreateRectangle((Vector2){ -size*0.35f, size*0.15f }, (Vector2){ size*0.15f, size*0.1f }, wheelColor);
    RayPals2DShape* backWheels = CreateRectangle((Vector2){ size*0.35f, size*0.15f }, (Vector2){ size*0.15f, size*0.1f }, wheelColor);
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, deck);
    AddShapeToSprite(sprite, frontWheels);
    AddShapeToSprite(sprite, backWheels);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateSword(Vector2 position, float size, Color bladeColor, Color hiltColor) {
    RayPalsSprite* sprite = CreateSprite(4);
    if (sprite == NULL) return NULL;
    
    // Modified sword with pointed tip
    
    // Blade - now with a pointed triangular tip
    RayPals2DShape* blade = CreateRectangle(
        (Vector2){ 0, -size*0.15f }, 
        (Vector2){ size*0.15f, size*0.6f }, 
        bladeColor
    );
    
    // Add a pointed tip (triangle)
    RayPals2DShape* tip = CreateTriangle(
        (Vector2){ 0, -size*0.5f }, 
        size*0.15f, 
        bladeColor
    );
    
    // Hilt/Handle
    RayPals2DShape* handle = CreateRectangle(
        (Vector2){ 0, size*0.3f }, 
        (Vector2){ size*0.08f, size*0.3f }, 
        hiltColor
    );
    
    // Guard (cross piece)
    RayPals2DShape* guard = CreateRectangle(
        (Vector2){ 0, size*0.1f }, 
        (Vector2){ size*0.4f, size*0.08f }, 
        hiltColor
    );
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, blade);
    AddShapeToSprite(sprite, tip);
    AddShapeToSprite(sprite, handle);
    AddShapeToSprite(sprite, guard);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateArrowSprite(Vector2 position, float size, Color shaftColor, Color headColor) {
    RayPalsSprite* sprite = CreateSprite(3);
    if (sprite == NULL) return NULL;
    
    // Arrow shaft
    RayPals2DShape* shaft = CreateRectangle((Vector2){ 0, 0 }, (Vector2){ size*0.8f, size*0.1f }, shaftColor);
    
    // Arrow head (triangle)
    RayPals2DShape* head = CreateTriangle((Vector2){ size*0.5f, 0 }, size*0.3f, headColor);
    head->rotation = 90.0f;  // Rotate to point rightward
    
    // Arrow fletching (back)
    RayPals2DShape* fletching = CreateTriangle((Vector2){ -size*0.4f, 0 }, size*0.2f, headColor);
    fletching->rotation = -90.0f;  // Rotate to point leftward
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, shaft);
    AddShapeToSprite(sprite, head);
    AddShapeToSprite(sprite, fletching);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateSailboat(Vector2 position, float size, Color hullColor, Color sailColor) {
    RayPalsSprite* sprite = CreateSprite(4);
    if (sprite == NULL) return NULL;
    
    // Boat hull (bottom half of a rectangle, with flat top)
    RayPals2DShape* hull = CreateRectangle((Vector2){ 0, size*0.2f }, (Vector2){ size, size*0.3f }, hullColor);
    
    // Sail (triangle)
    RayPals2DShape* sail = CreateTriangle((Vector2){ 0, -size*0.2f }, size*0.8f, sailColor);
    sail->rotation = 90.0f;  // Rotate for proper orientation
    
    // Mast (vertical pole)
    RayPals2DShape* mast = CreateRectangle((Vector2){ 0, 0 }, (Vector2){ size*0.05f, size*0.7f }, BROWN);
    
    // Water line (visual effect)
    RayPals2DShape* waterLine = CreateRectangle((Vector2){ 0, size*0.35f }, (Vector2){ size*1.1f, size*0.05f }, BLUE);
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, waterLine);
    AddShapeToSprite(sprite, hull);
    AddShapeToSprite(sprite, mast);
    AddShapeToSprite(sprite, sail);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

// Helper function to clamp color values
static unsigned char ClampColor(int value) {
    if (value < 0) return 0;
    if (value > 255) return 255;
    return (unsigned char)value;
}

RayPalsSprite* CreateStarSprite(Vector2 position, float size, Color color) {
    RayPalsSprite* sprite = CreateSprite(2);
    if (sprite == NULL) return NULL;
    
    // Default to yellow if color is not specified (has 0 values)
    if (color.r == 0 && color.g == 0 && color.b == 0 && color.a == 0) {
        color = YELLOW;  // Use raylib's yellow color
    }
    
    // Create a main 5-pointed star - making it bigger for better visibility
    RayPals2DShape* mainStar = CreateStar((Vector2){ 0, 0 }, size * 1.2f, 5, color);
    if (mainStar == NULL) {
        FreeSprite(sprite);
        return NULL;
    }
    
    // Create a smaller inner star with different color for visual interest
    Color innerColor = (Color){ 
        ClampColor(color.r + 40), 
        ClampColor(color.g + 40), 
        ClampColor(color.b + 40), 
        color.a 
    };
    
    // Use slightly larger inner star for better visibility
    RayPals2DShape* innerStar = CreateStar((Vector2){ 0, 0 }, size*0.7f, 5, innerColor);
    if (innerStar == NULL) {
        FreeShape(mainStar);
        FreeSprite(sprite);
        return NULL;
    }
    
    // Make sure stars are visible
    mainStar->visible = true;
    innerStar->visible = true;
    
    // Add shapes to sprite in right order (inner star on top)
    AddShapeToSprite(sprite, mainStar);
    AddShapeToSprite(sprite, innerStar);
    
    // Set sprite position
    sprite->position = position;
    sprite->visible = true;
    
    return sprite;
}

RayPalsSprite* CreateFlower(Vector2 position, float size, Color petalColor, Color centerColor) {
    RayPalsSprite* sprite = CreateSprite(6);
    if (sprite == NULL) return NULL;
    
    // Create flower center
    RayPals2DShape* center = CreateCircle((Vector2){ 0, 0 }, size*0.2f, centerColor);
    
    // Create petals (5 circles around the center)
    float petalSize = size*0.3f;
    float petalDistance = size*0.25f;
    
    RayPals2DShape* petal1 = CreateCircle((Vector2){ 0, -petalDistance }, petalSize, petalColor);
    RayPals2DShape* petal2 = CreateCircle((Vector2){ petalDistance, 0 }, petalSize, petalColor);
    RayPals2DShape* petal3 = CreateCircle((Vector2){ 0, petalDistance }, petalSize, petalColor);
    RayPals2DShape* petal4 = CreateCircle((Vector2){ -petalDistance, 0 }, petalSize, petalColor);
    RayPals2DShape* petal5 = CreateCircle((Vector2){ 0, 0 }, petalSize*1.3f, petalColor);
    petal5->filled = false;
    petal5->thickness = 2.0f;
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, petal1);
    AddShapeToSprite(sprite, petal2);
    AddShapeToSprite(sprite, petal3);
    AddShapeToSprite(sprite, petal4);
    AddShapeToSprite(sprite, petal5);
    AddShapeToSprite(sprite, center);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateCoin(Vector2 position, float size, Color color) {
    RayPalsSprite* sprite = CreateSprite(2);
    if (sprite == NULL) return NULL;
    
    // Coin base (circle)
    RayPals2DShape* coinBase = CreateCircle((Vector2){ 0, 0 }, size/2, color);
    
    // Coin border/detail (inner circle)
    Color darkerColor = (Color){ 
        (unsigned char)(color.r * 0.8f), 
        (unsigned char)(color.g * 0.8f), 
        (unsigned char)(color.b * 0.8f), 
        color.a 
    };
    
    RayPals2DShape* coinInner = CreateCircle((Vector2){ 0, 0 }, size*0.35f, darkerColor);
    coinInner->filled = false;
    coinInner->thickness = 2.0f;
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, coinBase);
    AddShapeToSprite(sprite, coinInner);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateKey(Vector2 position, float size, Color color) {
    RayPalsSprite* sprite = CreateSprite(3);
    if (sprite == NULL) return NULL;
    
    // Key handle (circle)
    RayPals2DShape* handle = CreateCircle((Vector2){ size*0.3f, 0 }, size*0.2f, color);
    
    // Key shaft (rectangle)
    RayPals2DShape* shaft = CreateRectangle((Vector2){ -size*0.15f, 0 }, (Vector2){ size*0.7f, size*0.1f }, color);
    
    // Key teeth (rectangle)
    RayPals2DShape* teeth = CreateRectangle((Vector2){ -size*0.4f, 0 }, (Vector2){ size*0.2f, size*0.3f }, color);
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, handle);
    AddShapeToSprite(sprite, shaft);
    AddShapeToSprite(sprite, teeth);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateShield(Vector2 position, float size, Color mainColor, Color borderColor) {
    RayPalsSprite* sprite = CreateSprite(2);
    if (sprite == NULL) return NULL;
    
    // Main shield body - simple inverted triangle
    RayPals2DShape* body = CreateTriangle((Vector2){ 0, 0 }, size, mainColor);
    body->rotation = 180.0f;  // Rotate to point downward
    
    // Shield border
    RayPals2DShape* border = CreateTriangle((Vector2){ 0, 0 }, size*1.05f, borderColor);
    border->rotation = 180.0f;  // Rotate to point downward
    border->filled = false;
    border->thickness = 2.0f;
    
    // Add shapes to sprite in drawing order
    AddShapeToSprite(sprite, border);
    AddShapeToSprite(sprite, body);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateCrown(Vector2 position, float size, Color color, Color gemColor) {
    RayPalsSprite* sprite = CreateSprite(7);
    if (sprite == NULL) return NULL;
    
    // Crown base (rectangle)
    RayPals2DShape* base = CreateRectangle((Vector2){ 0, size*0.1f }, (Vector2){ size, size*0.3f }, color);
    
    // Crown points (triangles)
    RayPals2DShape* point1 = CreateTriangle((Vector2){ -size*0.3f, -size*0.1f }, size*0.2f, color);
    RayPals2DShape* point2 = CreateTriangle((Vector2){ 0, -size*0.2f }, size*0.3f, color);
    RayPals2DShape* point3 = CreateTriangle((Vector2){ size*0.3f, -size*0.1f }, size*0.2f, color);
    
    // Crown gems (circles)
    RayPals2DShape* gem1 = CreateCircle((Vector2){ -size*0.3f, 0 }, size*0.1f, gemColor);
    RayPals2DShape* gem2 = CreateCircle((Vector2){ 0, -size*0.1f }, size*0.12f, gemColor);
    RayPals2DShape* gem3 = CreateCircle((Vector2){ size*0.3f, 0 }, size*0.1f, gemColor);
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, base);
    AddShapeToSprite(sprite, point1);
    AddShapeToSprite(sprite, point2);
    AddShapeToSprite(sprite, point3);
    AddShapeToSprite(sprite, gem1);
    AddShapeToSprite(sprite, gem2);
    AddShapeToSprite(sprite, gem3);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateLightningBolt(Vector2 position, float size, Color color) {
    RayPalsSprite* sprite = CreateSprite(7);
    if (sprite == NULL) return NULL;
    
    // Create a Harry Potter style lightning bolt
    // Using connected triangles for sharp zigzag
    float width = size * 0.2f;
    
    // Create zigzag segments using triangles
    // Define each segment as a rectangle
    
    // First segment (top-right)
    RayPals2DShape* seg1 = CreateRectangle(
        (Vector2){ -size*0.1f, -size*0.4f }, 
        (Vector2){ width, size*0.2f }, 
        color
    );
    seg1->rotation = -35.0f;
    
    // Second segment (middle-left)
    RayPals2DShape* seg2 = CreateRectangle(
        (Vector2){ -size*0.15f, -size*0.2f }, 
        (Vector2){ width, size*0.25f }, 
        color
    );
    seg2->rotation = 40.0f;
    
    // Third segment (middle-right)
    RayPals2DShape* seg3 = CreateRectangle(
        (Vector2){ 0, 0 }, 
        (Vector2){ width, size*0.2f }, 
        color
    );
    seg3->rotation = -35.0f;
    
    // Fourth segment (bottom-left)
    RayPals2DShape* seg4 = CreateRectangle(
        (Vector2){ 0, size*0.2f }, 
        (Vector2){ width, size*0.25f }, 
        color
    );
    seg4->rotation = 45.0f;
    
    // Add a glow effect with slightly larger, semi-transparent shapes
    Color glowColor = color;
    glowColor.a = 120; // Semi-transparent
    
    RayPals2DShape* glow1 = CreateRectangle(
        (Vector2){ -size*0.1f, -size*0.4f }, 
        (Vector2){ width*1.5f, size*0.2f*1.5f }, 
        glowColor
    );
    glow1->rotation = -35.0f;
    
    RayPals2DShape* glow2 = CreateRectangle(
        (Vector2){ 0, size*0.2f }, 
        (Vector2){ width*1.5f, size*0.25f*1.5f }, 
        glowColor
    );
    glow2->rotation = 45.0f;
    
    // Add shapes to sprite (glows first, then bolt)
    AddShapeToSprite(sprite, glow1);
    AddShapeToSprite(sprite, glow2);
    AddShapeToSprite(sprite, seg1);
    AddShapeToSprite(sprite, seg2);
    AddShapeToSprite(sprite, seg3);
    AddShapeToSprite(sprite, seg4);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateFish(Vector2 position, float size, Color bodyColor, Color finColor) {
    RayPalsSprite* sprite = CreateSprite(4);
    if (sprite == NULL) return NULL;
    
    // Fish body (oval/ellipse using a circle with scaling)
    RayPals2DShape* body = CreateCircle((Vector2){ 0, 0 }, size*0.4f, bodyColor);
    
    // Fish tail (triangle)
    RayPals2DShape* tail = CreateTriangle((Vector2){ size*0.4f, 0 }, size*0.3f, finColor);
    tail->rotation = 90.0f;
    
    // Fish fins (triangles)
    RayPals2DShape* topFin = CreateTriangle((Vector2){ 0, -size*0.3f }, size*0.2f, finColor);
    RayPals2DShape* bottomFin = CreateTriangle((Vector2){ 0, size*0.3f }, size*0.2f, finColor);
    bottomFin->rotation = 180.0f;
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, tail);
    AddShapeToSprite(sprite, body);
    AddShapeToSprite(sprite, topFin);
    AddShapeToSprite(sprite, bottomFin);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateGem(Vector2 position, float size, Color color) {
    RayPalsSprite* sprite = CreateSprite(4);
    if (sprite == NULL) return NULL;
    
    // Create a diamond shape - main body
    RayPals2DShape* mainGem = CreatePolygon((Vector2){ 0, 0 }, size/2, 8, color);
    mainGem->rotation = 22.5f;  // Rotate to get diamond orientation
    
    // Create a smaller interior with lighter color for "sparkle"
    Color lighterColor = (Color){ 
        ClampColor(color.r + 80), 
        ClampColor(color.g + 80), 
        ClampColor(color.b + 80), 
        color.a 
    };
    
    RayPals2DShape* innerGem = CreatePolygon((Vector2){ -size*0.05f, -size*0.05f }, size/2*0.6f, 8, lighterColor);
    innerGem->rotation = 22.5f;
    
    // Add a small star for sparkle effect
    RayPals2DShape* sparkle = CreateStar((Vector2){ -size*0.15f, -size*0.15f }, size*0.15f, 4, WHITE);
    
    // Add border outline
    RayPals2DShape* border = CreatePolygon((Vector2){ 0, 0 }, size/2*1.02f, 8, BLACK);
    border->filled = false;
    border->thickness = 1.5f;
    border->rotation = 22.5f;
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, border);
    AddShapeToSprite(sprite, mainGem);
    AddShapeToSprite(sprite, innerGem);
    AddShapeToSprite(sprite, sparkle);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

// ----------------------------------------------------------------------------
// Animation Functions
// ----------------------------------------------------------------------------

// UpdateShapeAnimation and Update3DShapeAnimation functions are already defined above

// The CreateWaterDrop function is already defined earlier in the file (line ~230)
// No duplicate implementation needed here

RayPalsSprite* CreateWaterDropSprite(Vector2 position, float size, Color color) {
    RayPalsSprite* sprite = CreateSprite(2);
    if (sprite == NULL) return NULL;
    
    // Create the drop shape using a circle for the main part and a triangle for the pointed end
    RayPals2DShape* dropBody = CreateCircle((Vector2){ 0, -size*0.1f }, size*0.4f, color);
    
    // Create drop point (triangle)
    RayPals2DShape* dropPoint = CreateTriangle((Vector2){ 0, size*0.3f }, size*0.4f, color);
    dropPoint->rotation = 180.0f; // Point facing downward
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, dropBody);
    AddShapeToSprite(sprite, dropPoint);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateHorseSprite(Vector2 position, float size, Color bodyColor) {
    RayPalsSprite* sprite = CreateSprite(6);
    if (sprite == NULL) return NULL;
    
    // Body - slightly elongated rectangle
    RayPals2DShape* body = CreateRectangle(
        (Vector2){ 0, 0 }, 
        (Vector2){ size*1.8f, size*0.8f }, 
        bodyColor
    );
    
    // Head - rectangle with rotation
    RayPals2DShape* head = CreateRectangle(
        (Vector2){ size*0.9f, -size*0.6f }, 
        (Vector2){ size*0.8f, size*0.4f }, 
        bodyColor
    );
    head->rotation = 45.0f;
    
    // Neck - connecting body and head
    RayPals2DShape* neck = CreateRectangle(
        (Vector2){ size*0.6f, -size*0.3f }, 
        (Vector2){ size*0.4f, size*0.7f }, 
        bodyColor
    );
    neck->rotation = 30.0f;
    
    // Tail
    RayPals2DShape* tail = CreateRectangle(
        (Vector2){ -size*0.9f, -size*0.4f }, 
        (Vector2){ size*0.5f, size*0.2f }, 
        bodyColor
    );
    tail->rotation = -35.0f;
    
    // Legs (front and back)
    RayPals2DShape* frontLeg = CreateRectangle(
        (Vector2){ size*0.5f, size*0.6f }, 
        (Vector2){ size*0.2f, size*0.8f }, 
        bodyColor
    );
    
    RayPals2DShape* backLeg = CreateRectangle(
        (Vector2){ -size*0.5f, size*0.6f }, 
        (Vector2){ size*0.2f, size*0.8f }, 
        bodyColor
    );
    
    // Add shapes to sprite in proper order
    AddShapeToSprite(sprite, tail);
    AddShapeToSprite(sprite, body);
    AddShapeToSprite(sprite, neck);
    AddShapeToSprite(sprite, head);
    AddShapeToSprite(sprite, frontLeg);
    AddShapeToSprite(sprite, backLeg);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateYellowStar(Vector2 position, float size) {
    // Create a bright yellow star that's clearly visible
    Color brightYellow = (Color){ 255, 255, 0, 255 }; // Maximum brightness yellow
    
    // Create a star with yellow color and increased size for visibility
    RayPalsSprite* star = CreateStarSprite(position, size * 1.2f, brightYellow);
    
    // Apply additional settings if needed
    if (star) {
        // Ensure the star is visible
        star->visible = true;
    }
    
    return star;
}

RayPalsSprite* CreateExplosion(Vector2 position, float size, Color primaryColor, Color secondaryColor) {
    RayPalsSprite* sprite = CreateSprite(8);
    if (sprite == NULL) return NULL;
    
    // Create center of explosion
    RayPals2DShape* center = CreateCircle((Vector2){ 0, 0 }, size * 0.4f, primaryColor);
    
    // Create outer explosion circles
    RayPals2DShape* outerCircle1 = CreateCircle((Vector2){ size * 0.3f, size * 0.2f }, size * 0.25f, secondaryColor);
    RayPals2DShape* outerCircle2 = CreateCircle((Vector2){ -size * 0.25f, size * 0.3f }, size * 0.2f, secondaryColor);
    RayPals2DShape* outerCircle3 = CreateCircle((Vector2){ -size * 0.3f, -size * 0.2f }, size * 0.25f, secondaryColor);
    RayPals2DShape* outerCircle4 = CreateCircle((Vector2){ size * 0.25f, -size * 0.3f }, size * 0.2f, secondaryColor);
    
    // Create some triangular "spikes" for a more dynamic look
    RayPals2DShape* spike1 = CreateTriangle((Vector2){ size * 0.45f, 0 }, size * 0.4f, primaryColor);
    spike1->rotation = 0.0f;
    
    RayPals2DShape* spike2 = CreateTriangle((Vector2){ 0, size * 0.45f }, size * 0.4f, primaryColor);
    spike2->rotation = 90.0f;
    
    RayPals2DShape* spike3 = CreateTriangle((Vector2){ -size * 0.45f, 0 }, size * 0.4f, primaryColor);
    spike3->rotation = 180.0f;
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, outerCircle1);
    AddShapeToSprite(sprite, outerCircle2);
    AddShapeToSprite(sprite, outerCircle3);
    AddShapeToSprite(sprite, outerCircle4);
    AddShapeToSprite(sprite, spike1);
    AddShapeToSprite(sprite, spike2);
    AddShapeToSprite(sprite, spike3);
    AddShapeToSprite(sprite, center);  // Center on top for better visual
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateAirplane(Vector2 position, float size, Color bodyColor, Color detailColor) {
    RayPalsSprite* sprite = CreateSprite(6);
    if (sprite == NULL) return NULL;
    
    // Airplane fuselage (main body)
    RayPals2DShape* body = CreateRectangle((Vector2){ 0, 0 }, (Vector2){ size * 1.2f, size * 0.25f }, bodyColor);
    
    // Airplane nose (front part)
    RayPals2DShape* nose = CreateTriangle((Vector2){ size * 0.6f, 0 }, size * 0.4f, bodyColor);
    nose->rotation = 90.0f;  // Point to the right
    
    // Airplane tail (back part)
    RayPals2DShape* tail = CreateRectangle((Vector2){ -size * 0.5f, -size * 0.25f }, (Vector2){ size * 0.3f, size * 0.3f }, bodyColor);
    
    // Airplane wings
    RayPals2DShape* wings = CreateRectangle((Vector2){ 0, 0 }, (Vector2){ size * 0.6f, size * 0.8f }, bodyColor);
    
    // Windows (detail)
    RayPals2DShape* window1 = CreateCircle((Vector2){ size * 0.2f, 0 }, size * 0.05f, detailColor);
    RayPals2DShape* window2 = CreateCircle((Vector2){ 0, 0 }, size * 0.05f, detailColor);
    RayPals2DShape* window3 = CreateCircle((Vector2){ -size * 0.2f, 0 }, size * 0.05f, detailColor);
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, body);
    AddShapeToSprite(sprite, wings);
    AddShapeToSprite(sprite, nose);
    AddShapeToSprite(sprite, tail);
    AddShapeToSprite(sprite, window1);
    AddShapeToSprite(sprite, window2);
    AddShapeToSprite(sprite, window3);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateSoldier(Vector2 position, float size, Color uniformColor, Color skinColor) {
    RayPalsSprite* sprite = CreateSprite(7);
    if (sprite == NULL) return NULL;
    
    // Body parts
    RayPals2DShape* body = CreateRectangle((Vector2){ 0, size * 0.15f }, (Vector2){ size * 0.4f, size * 0.5f }, uniformColor);
    
    // Head with helmet
    RayPals2DShape* head = CreateCircle((Vector2){ 0, -size * 0.25f }, size * 0.2f, skinColor);
    RayPals2DShape* helmet = CreateRectangle((Vector2){ 0, -size * 0.35f }, (Vector2){ size * 0.4f, size * 0.15f }, uniformColor);
    
    // Arms
    RayPals2DShape* leftArm = CreateRectangle((Vector2){ -size * 0.25f, size * 0.1f }, (Vector2){ size * 0.1f, size * 0.4f }, uniformColor);
    leftArm->rotation = -15.0f;
    
    RayPals2DShape* rightArm = CreateRectangle((Vector2){ size * 0.25f, size * 0.1f }, (Vector2){ size * 0.1f, size * 0.4f }, uniformColor);
    rightArm->rotation = 15.0f;
    
    // Legs
    RayPals2DShape* leftLeg = CreateRectangle((Vector2){ -size * 0.15f, size * 0.5f }, (Vector2){ size * 0.15f, size * 0.4f }, uniformColor);
    RayPals2DShape* rightLeg = CreateRectangle((Vector2){ size * 0.15f, size * 0.5f }, (Vector2){ size * 0.15f, size * 0.4f }, uniformColor);
    
    // Gun (optional)
    RayPals2DShape* gun = CreateRectangle((Vector2){ size * 0.4f, size * 0.2f }, (Vector2){ size * 0.5f, size * 0.1f }, (Color){ 80, 80, 80, 255 });
    
    // Add shapes to sprite in correct order
    AddShapeToSprite(sprite, leftLeg);
    AddShapeToSprite(sprite, rightLeg);
    AddShapeToSprite(sprite, body);
    AddShapeToSprite(sprite, leftArm);
    AddShapeToSprite(sprite, rightArm);
    AddShapeToSprite(sprite, head);
    AddShapeToSprite(sprite, helmet);
    AddShapeToSprite(sprite, gun);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateZombie(Vector2 position, float size, Color skinColor, Color clothesColor) {
    RayPalsSprite* sprite = CreateSprite(8);
    if (sprite == NULL) return NULL;
    
    // Create zombie with hunched posture and tattered clothes
    
    // Body - slightly tilted
    RayPals2DShape* body = CreateRectangle((Vector2){ 0, size * 0.1f }, (Vector2){ size * 0.4f, size * 0.5f }, clothesColor);
    body->rotation = 10.0f;  // Slight tilt
    
    // Tattered clothes effect
    RayPals2DShape* tatters = CreateTriangle((Vector2){ size * 0.1f, size * 0.3f }, size * 0.3f, clothesColor);
    tatters->rotation = 180.0f;
    
    // Head - green/gray skin
    Color zombieSkin = skinColor;
    zombieSkin.g += 40;  // Add greenish tint
    zombieSkin.r -= 30;  // Reduce red for a sickly look
    
    RayPals2DShape* head = CreateCircle((Vector2){ size * 0.05f, -size * 0.25f }, size * 0.2f, zombieSkin);
    
    // Eyes - creepy white
    RayPals2DShape* leftEye = CreateCircle((Vector2){ -size * 0.05f, -size * 0.27f }, size * 0.05f, WHITE);
    RayPals2DShape* rightEye = CreateCircle((Vector2){ size * 0.12f, -size * 0.27f }, size * 0.05f, WHITE);
    
    // Arms - asymmetrical and outstretched
    RayPals2DShape* leftArm = CreateRectangle((Vector2){ -size * 0.3f, size * 0.0f }, (Vector2){ size * 0.2f, size * 0.1f }, zombieSkin);
    leftArm->rotation = -30.0f;
    
    RayPals2DShape* rightArm = CreateRectangle((Vector2){ size * 0.2f, size * 0.1f }, (Vector2){ size * 0.3f, size * 0.1f }, zombieSkin);
    rightArm->rotation = 20.0f;
    
    // Legs - one dragging
    RayPals2DShape* leftLeg = CreateRectangle((Vector2){ -size * 0.15f, size * 0.45f }, (Vector2){ size * 0.15f, size * 0.4f }, clothesColor);
    leftLeg->rotation = -5.0f;
    
    RayPals2DShape* rightLeg = CreateRectangle((Vector2){ size * 0.12f, size * 0.5f }, (Vector2){ size * 0.15f, size * 0.45f }, clothesColor);
    rightLeg->rotation = 15.0f;  // Dragging leg
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, rightLeg);
    AddShapeToSprite(sprite, leftLeg);
    AddShapeToSprite(sprite, body);
    AddShapeToSprite(sprite, tatters);
    AddShapeToSprite(sprite, leftArm);
    AddShapeToSprite(sprite, rightArm);
    AddShapeToSprite(sprite, head);
    AddShapeToSprite(sprite, leftEye);
    AddShapeToSprite(sprite, rightEye);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateUFO(Vector2 position, float size, Color bodyColor, Color glowColor) {
    RayPalsSprite* sprite = CreateSprite(5);
    if (sprite == NULL) return NULL;
    
    // Main UFO saucer body - elliptical shape
    RayPals2DShape* body = CreateCircle((Vector2){ 0, 0 }, size * 0.5f, bodyColor);
    
    // Top dome of the UFO
    RayPals2DShape* dome = CreateCircle((Vector2){ 0, -size * 0.15f }, size * 0.25f, bodyColor);
    
    // Bottom rim of the UFO
    RayPals2DShape* bottomRim = CreateRectangle(
        (Vector2){ 0, size * 0.05f }, 
        (Vector2){ size * 0.8f, size * 0.1f }, 
        DARKGRAY
    );
    
    // Glowing lights around the rim
    Color lightColor = glowColor;
    lightColor.a = 220; // Semi-transparent for glow effect
    
    RayPals2DShape* lights1 = CreateCircle((Vector2){ -size * 0.25f, size * 0.05f }, size * 0.08f, lightColor);
    RayPals2DShape* lights2 = CreateCircle((Vector2){ 0, size * 0.05f }, size * 0.08f, lightColor);
    RayPals2DShape* lights3 = CreateCircle((Vector2){ size * 0.25f, size * 0.05f }, size * 0.08f, lightColor);
    
    // Optional tractor beam effect
    RayPals2DShape* beam = CreateTriangle(
        (Vector2){ 0, size * 0.2f },
        size * 0.5f,
        (Color){ glowColor.r, glowColor.g, glowColor.b, 100 } // Very transparent
    );
    beam->rotation = 180.0f; // Point downward
    
    // Add shapes to sprite in appropriate order
    AddShapeToSprite(sprite, beam);
    AddShapeToSprite(sprite, body);
    AddShapeToSprite(sprite, bottomRim);
    AddShapeToSprite(sprite, lights1);
    AddShapeToSprite(sprite, lights2);
    AddShapeToSprite(sprite, lights3);
    AddShapeToSprite(sprite, dome);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateDragon(Vector2 position, float size, Color bodyColor, Color wingColor) {
    RayPalsSprite* sprite = CreateSprite(9);
    if (sprite == NULL) return NULL;
    
    // Dragon body (main elongated shape)
    RayPals2DShape* body = CreateRectangle(
        (Vector2){ 0, 0 }, 
        (Vector2){ size * 1.2f, size * 0.4f }, 
        bodyColor
    );
    
    // Dragon head (triangle for snout effect)
    RayPals2DShape* head = CreateTriangle(
        (Vector2){ size * 0.7f, -size * 0.1f }, 
        size * 0.4f, 
        bodyColor
    );
    head->rotation = 90.0f; // Point to the right
    
    // Dragon neck
    RayPals2DShape* neck = CreateRectangle(
        (Vector2){ size * 0.4f, -size * 0.1f }, 
        (Vector2){ size * 0.3f, size * 0.2f }, 
        bodyColor
    );
    
    // Dragon tail (curved)
    RayPals2DShape* tail1 = CreateRectangle(
        (Vector2){ -size * 0.5f, size * 0.0f }, 
        (Vector2){ size * 0.4f, size * 0.15f }, 
        bodyColor
    );
    tail1->rotation = -15.0f;
    
    RayPals2DShape* tail2 = CreateTriangle(
        (Vector2){ -size * 0.8f, size * 0.1f }, 
        size * 0.3f, 
        bodyColor
    );
    tail2->rotation = -90.0f; // Point to the left
    
    // Dragon wings
    RayPals2DShape* leftWing = CreateTriangle(
        (Vector2){ size * 0.1f, -size * 0.3f }, 
        size * 0.6f, 
        wingColor
    );
    leftWing->rotation = 30.0f;
    
    RayPals2DShape* rightWing = CreateTriangle(
        (Vector2){ -size * 0.1f, -size * 0.3f }, 
        size * 0.6f, 
        wingColor
    );
    rightWing->rotation = 150.0f;
    
    // Dragon eyes
    Color eyeColor = RED;
    RayPals2DShape* leftEye = CreateCircle(
        (Vector2){ size * 0.6f, -size * 0.15f }, 
        size * 0.05f, 
        eyeColor
    );
    
    // Dragon horns
    RayPals2DShape* horn1 = CreateTriangle(
        (Vector2){ size * 0.65f, -size * 0.25f }, 
        size * 0.2f, 
        bodyColor
    );
    horn1->rotation = 45.0f;
    
    RayPals2DShape* horn2 = CreateTriangle(
        (Vector2){ size * 0.55f, -size * 0.25f }, 
        size * 0.2f, 
        bodyColor
    );
    horn2->rotation = 15.0f;
    
    // Add shapes to sprite in proper order
    AddShapeToSprite(sprite, leftWing);
    AddShapeToSprite(sprite, rightWing);
    AddShapeToSprite(sprite, tail2);
    AddShapeToSprite(sprite, tail1);
    AddShapeToSprite(sprite, body);
    AddShapeToSprite(sprite, neck);
    AddShapeToSprite(sprite, head);
    AddShapeToSprite(sprite, horn1);
    AddShapeToSprite(sprite, horn2);
    AddShapeToSprite(sprite, leftEye);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateWizard(Vector2 position, float size, Color robeColor, Color hatColor) {
    RayPalsSprite* sprite = CreateSprite(8);
    if (sprite == NULL) return NULL;
    
    // Colors
    Color faceColor = BEIGE;
    Color staffColor = BROWN;
    Color starColor = YELLOW;
    
    // Wizard robe (base)
    RayPals2DShape* robe = CreateTriangle(
        (Vector2){ 0, size * 0.3f }, 
        size * 0.9f, 
        robeColor
    );
    robe->rotation = 180.0f; // Point downward
    
    // Wizard body (upper part)
    RayPals2DShape* body = CreateRectangle(
        (Vector2){ 0, -size * 0.1f }, 
        (Vector2){ size * 0.4f, size * 0.5f }, 
        robeColor
    );
    
    // Wizard head
    RayPals2DShape* head = CreateCircle(
        (Vector2){ 0, -size * 0.4f }, 
        size * 0.15f, 
        faceColor
    );
    
    // Wizard hat (cone)
    RayPals2DShape* hat = CreateTriangle(
        (Vector2){ 0, -size * 0.65f }, 
        size * 0.5f, 
        hatColor
    );
    hat->rotation = 0.0f; // Point upward
    
    // Wizard hat brim
    RayPals2DShape* hatBrim = CreateRectangle(
        (Vector2){ 0, -size * 0.5f }, 
        (Vector2){ size * 0.45f, size * 0.05f }, 
        hatColor
    );
    
    // Wizard beard
    RayPals2DShape* beard = CreateTriangle(
        (Vector2){ 0, -size * 0.3f }, 
        size * 0.25f, 
        WHITE
    );
    beard->rotation = 180.0f; // Point downward
    
    // Wizard staff
    RayPals2DShape* staff = CreateRectangle(
        (Vector2){ size * 0.3f, size * 0.1f }, 
        (Vector2){ size * 0.05f, size * 0.8f }, 
        staffColor
    );
    staff->rotation = -15.0f;
    
    // Wizard staff star
    RayPals2DShape* staffStar = CreateStar(
        (Vector2){ size * 0.35f, -size * 0.25f }, 
        size * 0.15f, 
        5, 
        starColor
    );
    
    // Add shapes to sprite in proper order
    AddShapeToSprite(sprite, staff);
    AddShapeToSprite(sprite, robe);
    AddShapeToSprite(sprite, body);
    AddShapeToSprite(sprite, head);
    AddShapeToSprite(sprite, beard);
    AddShapeToSprite(sprite, hatBrim);
    AddShapeToSprite(sprite, hat);
    AddShapeToSprite(sprite, staffStar);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateTreasureChest(Vector2 position, float size, Color chestColor, Color goldColor, bool isOpen) {
    RayPalsSprite* sprite = CreateSprite(7);
    if (sprite == NULL) return NULL;
    
    // Colors
    Color lockColor = DARKBROWN;
    
    // Chest base (main rectangle)
    RayPals2DShape* base = CreateRectangle(
        (Vector2){ 0, size * 0.1f }, 
        (Vector2){ size * 0.8f, size * 0.5f }, 
        chestColor
    );
    
    // Chest lid
    RayPals2DShape* lid = CreateRectangle(
        isOpen ? (Vector2){ -size * 0.3f, -size * 0.25f } : (Vector2){ 0, -size * 0.15f }, 
        (Vector2){ size * 0.8f, size * 0.3f }, 
        chestColor
    );
    // Rotate the lid if open
    lid->rotation = isOpen ? -60.0f : 0.0f;
    
    // Chest lock/clasp
    RayPals2DShape* lock = CreateRectangle(
        isOpen ? (Vector2){ -size * 0.3f, -size * 0.1f } : (Vector2){ 0, 0 }, 
        (Vector2){ size * 0.15f, size * 0.15f }, 
        lockColor
    );
    // Rotate the lock with the lid if open
    lock->rotation = isOpen ? -60.0f : 0.0f;
    
    // Decorative elements (edges/corners)
    RayPals2DShape* corner1 = CreateRectangle(
        (Vector2){ size * 0.35f, size * 0.15f }, 
        (Vector2){ size * 0.1f, size * 0.4f }, 
        lockColor
    );
    
    RayPals2DShape* corner2 = CreateRectangle(
        (Vector2){ -size * 0.35f, size * 0.15f }, 
        (Vector2){ size * 0.1f, size * 0.4f }, 
        lockColor
    );
    
    // Only create and add gold if chest is open
    RayPals2DShape* gold1 = NULL;
    RayPals2DShape* gold2 = NULL;
    
    if (isOpen) {
        // Gold coins/treasure (circles) that are visible when open
        gold1 = CreateCircle(
            (Vector2){ -size * 0.1f, size * 0.0f }, 
            size * 0.15f, 
            goldColor
        );
        
        gold2 = CreateCircle(
            (Vector2){ size * 0.15f, size * 0.05f }, 
            size * 0.12f, 
            goldColor
        );
    }
    
    // Add shapes to sprite in proper order
    AddShapeToSprite(sprite, base);
    AddShapeToSprite(sprite, corner1);
    AddShapeToSprite(sprite, corner2);
    
    // Add gold pieces if chest is open
    if (isOpen) {
        AddShapeToSprite(sprite, gold1);
        AddShapeToSprite(sprite, gold2);
    }
    
    // Add lid and lock last so they appear on top
    AddShapeToSprite(sprite, lid);
    AddShapeToSprite(sprite, lock);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreatePortal(Vector2 position, float size, Color outerColor, Color innerColor) {
    RayPalsSprite* sprite = CreateSprite(8);
    if (sprite == NULL) return NULL;
    
    // Create a swirling portal with multiple rings
    
    // Outer ring
    RayPals2DShape* outerRing = CreateCircle(
        (Vector2){ 0, 0 }, 
        size * 0.5f, 
        outerColor
    );
    outerRing->filled = false;
    outerRing->thickness = 4.0f;
    
    // Create several inner rings with decreasing size and alternating colors
    RayPals2DShape* ring1 = CreateCircle(
        (Vector2){ 0, 0 }, 
        size * 0.45f, 
        innerColor
    );
    ring1->filled = false;
    ring1->thickness = 3.0f;
    
    RayPals2DShape* ring2 = CreateCircle(
        (Vector2){ 0, 0 }, 
        size * 0.4f, 
        outerColor
    );
    ring2->filled = false;
    ring2->thickness = 3.0f;
    
    RayPals2DShape* ring3 = CreateCircle(
        (Vector2){ 0, 0 }, 
        size * 0.35f, 
        innerColor
    );
    ring3->filled = false;
    ring3->thickness = 3.0f;
    
    RayPals2DShape* ring4 = CreateCircle(
        (Vector2){ 0, 0 }, 
        size * 0.3f, 
        outerColor
    );
    ring4->filled = false;
    ring4->thickness = 3.0f;
    
    // Inner swirl (spiral effect using shapes)
    RayPals2DShape* swirl1 = CreateCircle(
        (Vector2){ size * 0.05f, size * 0.05f }, 
        size * 0.25f, 
        innerColor
    );
    swirl1->filled = true;
    
    RayPals2DShape* swirl2 = CreateCircle(
        (Vector2){ -size * 0.08f, -size * 0.08f }, 
        size * 0.15f, 
        outerColor
    );
    swirl2->filled = true;
    
    // Core of the portal 
    RayPals2DShape* core = CreateCircle(
        (Vector2){ 0, 0 }, 
        size * 0.1f, 
        (Color){ 255, 255, 255, 200 }  // Bright center with some transparency
    );
    
    // Add shapes to sprite in proper order (from back to front)
    AddShapeToSprite(sprite, outerRing);
    AddShapeToSprite(sprite, ring1);
    AddShapeToSprite(sprite, ring2);
    AddShapeToSprite(sprite, ring3);
    AddShapeToSprite(sprite, ring4);
    AddShapeToSprite(sprite, swirl1);
    AddShapeToSprite(sprite, swirl2);
    AddShapeToSprite(sprite, core);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateSnowman(Vector2 position, float size, Color snowColor, Color accessoryColor) {
    RayPalsSprite* sprite = CreateSprite(8);
    if (sprite == NULL) return NULL;
    
    // Bottom snowball (largest)
    RayPals2DShape* bottom = CreateCircle(
        (Vector2){ 0, size * 0.4f }, 
        size * 0.4f, 
        snowColor
    );
    
    // Middle snowball
    RayPals2DShape* middle = CreateCircle(
        (Vector2){ 0, 0 }, 
        size * 0.3f, 
        snowColor
    );
    
    // Head (top snowball - smallest)
    RayPals2DShape* head = CreateCircle(
        (Vector2){ 0, -size * 0.35f }, 
        size * 0.2f, 
        snowColor
    );
    
    // Eyes
    Color eyeColor = BLACK;
    RayPals2DShape* leftEye = CreateCircle(
        (Vector2){ -size * 0.08f, -size * 0.38f }, 
        size * 0.03f, 
        eyeColor
    );
    
    RayPals2DShape* rightEye = CreateCircle(
        (Vector2){ size * 0.08f, -size * 0.38f }, 
        size * 0.03f, 
        eyeColor
    );
    
    // Carrot nose
    Color carrotColor = ORANGE;
    RayPals2DShape* nose = CreateTriangle(
        (Vector2){ size * 0.1f, -size * 0.35f }, 
        size * 0.15f, 
        carrotColor
    );
    nose->rotation = 90.0f; // Point to the right
    
    // Hat (top hat)
    RayPals2DShape* hatBrim = CreateRectangle(
        (Vector2){ 0, -size * 0.55f }, 
        (Vector2){ size * 0.5f, size * 0.05f }, 
        accessoryColor
    );
    
    RayPals2DShape* hatTop = CreateRectangle(
        (Vector2){ 0, -size * 0.65f }, 
        (Vector2){ size * 0.3f, size * 0.2f }, 
        accessoryColor
    );
    
    // Arms (sticks)
    RayPals2DShape* leftArm = CreateRectangle(
        (Vector2){ -size * 0.4f, -size * 0.05f }, 
        (Vector2){ size * 0.4f, size * 0.03f }, 
        BROWN
    );
    leftArm->rotation = 20.0f;
    
    RayPals2DShape* rightArm = CreateRectangle(
        (Vector2){ size * 0.4f, -size * 0.05f }, 
        (Vector2){ size * 0.4f, size * 0.03f }, 
        BROWN
    );
    rightArm->rotation = -20.0f;
    
    // Add shapes to sprite in proper order
    AddShapeToSprite(sprite, bottom);
    AddShapeToSprite(sprite, leftArm);
    AddShapeToSprite(sprite, rightArm);
    AddShapeToSprite(sprite, middle);
    AddShapeToSprite(sprite, head);
    AddShapeToSprite(sprite, leftEye);
    AddShapeToSprite(sprite, rightEye);
    AddShapeToSprite(sprite, nose);
    AddShapeToSprite(sprite, hatBrim);
    AddShapeToSprite(sprite, hatTop);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreatePotion(Vector2 position, float size, Color bottleColor, Color liquidColor) {
    RayPalsSprite* sprite = CreateSprite(6);
    if (sprite == NULL) return NULL;
    
    // Potion bottle neck
    RayPals2DShape* bottleNeck = CreateRectangle(
        (Vector2){ 0, -size * 0.4f }, 
        (Vector2){ size * 0.15f, size * 0.2f }, 
        bottleColor
    );
    
    // Potion bottle cap/cork
    RayPals2DShape* bottleCap = CreateRectangle(
        (Vector2){ 0, -size * 0.55f }, 
        (Vector2){ size * 0.2f, size * 0.1f }, 
        BROWN
    );
    
    // Potion bottle body (wider at the bottom)
    RayPals2DShape* bottleTop = CreateRectangle(
        (Vector2){ 0, -size * 0.25f }, 
        (Vector2){ size * 0.3f, size * 0.1f }, 
        bottleColor
    );
    
    RayPals2DShape* bottleBody = CreateRectangle(
        (Vector2){ 0, size * 0.05f }, 
        (Vector2){ size * 0.5f, size * 0.5f }, 
        bottleColor
    );
    
    // Round bottom of the bottle
    RayPals2DShape* bottleBottom = CreateCircle(
        (Vector2){ 0, size * 0.3f }, 
        size * 0.25f, 
        bottleColor
    );
    
    // Liquid inside the bottle
    RayPals2DShape* liquid = CreateRectangle(
        (Vector2){ 0, size * 0.1f }, 
        (Vector2){ size * 0.45f, size * 0.4f }, 
        liquidColor
    );
    
    // Bubble effects in the liquid
    RayPals2DShape* bubble1 = CreateCircle(
        (Vector2){ -size * 0.1f, 0 }, 
        size * 0.05f, 
        (Color){ 255, 255, 255, 150 } // Semi-transparent white
    );
    
    RayPals2DShape* bubble2 = CreateCircle(
        (Vector2){ size * 0.15f, size * 0.15f }, 
        size * 0.03f, 
        (Color){ 255, 255, 255, 150 } // Semi-transparent white
    );
    
    // Add shapes to sprite in proper order
    AddShapeToSprite(sprite, bottleBottom);
    AddShapeToSprite(sprite, bottleBody);
    AddShapeToSprite(sprite, liquid);
    AddShapeToSprite(sprite, bubble1);
    AddShapeToSprite(sprite, bubble2);
    AddShapeToSprite(sprite, bottleTop);
    AddShapeToSprite(sprite, bottleNeck);
    AddShapeToSprite(sprite, bottleCap);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateCannon(Vector2 position, float size, Color cannonColor, Color wheelColor) {
    RayPalsSprite* sprite = CreateSprite(7);
    if (sprite == NULL) return NULL;
    
    // Cannon barrel (main part)
    RayPals2DShape* barrel = CreateRectangle(
        (Vector2){ size * 0.2f, -size * 0.1f }, 
        (Vector2){ size * 0.8f, size * 0.25f }, 
        cannonColor
    );
    
    // Cannon end (muzzle)
    RayPals2DShape* muzzle = CreateRectangle(
        (Vector2){ size * 0.6f, -size * 0.1f }, 
        (Vector2){ size * 0.1f, size * 0.3f }, 
        cannonColor
    );
    
    // Cannon base (carriage)
    RayPals2DShape* base = CreateRectangle(
        (Vector2){ 0, size * 0.1f }, 
        (Vector2){ size * 0.6f, size * 0.3f }, 
        cannonColor
    );
    
    // Cannon wheels
    RayPals2DShape* leftWheel = CreateCircle(
        (Vector2){ -size * 0.2f, size * 0.3f }, 
        size * 0.2f, 
        wheelColor
    );
    
    RayPals2DShape* rightWheel = CreateCircle(
        (Vector2){ size * 0.2f, size * 0.3f }, 
        size * 0.2f, 
        wheelColor
    );
    
    // Wheel hubs (decorative circles in the center of wheels)
    RayPals2DShape* leftHub = CreateCircle(
        (Vector2){ -size * 0.2f, size * 0.3f }, 
        size * 0.05f, 
        cannonColor
    );
    
    RayPals2DShape* rightHub = CreateCircle(
        (Vector2){ size * 0.2f, size * 0.3f }, 
        size * 0.05f, 
        cannonColor
    );
    
    // Optional - cannon ball inside the barrel
    RayPals2DShape* cannonBall = CreateCircle(
        (Vector2){ size * 0.4f, -size * 0.1f }, 
        size * 0.1f, 
        BLACK
    );
    
    // Add shapes to sprite in proper order
    AddShapeToSprite(sprite, leftWheel);
    AddShapeToSprite(sprite, rightWheel);
    AddShapeToSprite(sprite, base);
    AddShapeToSprite(sprite, barrel);
    AddShapeToSprite(sprite, muzzle);
    AddShapeToSprite(sprite, cannonBall);
    AddShapeToSprite(sprite, leftHub);
    AddShapeToSprite(sprite, rightHub);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

// ----------------------------------------------------------------------------
// 3D Sprite Functions
// ----------------------------------------------------------------------------

RayPals3DSprite* Create3DSprite(int initialCapacity) {
    RayPals3DSprite* sprite = (RayPals3DSprite*)malloc(sizeof(RayPals3DSprite));
    if (sprite == NULL) return NULL;
    
    sprite->shapes = (RayPals3DShape**)malloc(sizeof(RayPals3DShape*) * initialCapacity);
    if (sprite->shapes == NULL) {
        free(sprite);
        return NULL;
    }
    
    sprite->shapeCount = 0;
    sprite->position = (Vector3){ 0, 0, 0 };
    sprite->rotation = (Vector3){ 0, 0, 0 };
    sprite->scale = (Vector3){ 1, 1, 1 };
    sprite->visible = true;
    
    return sprite;
}

void AddShapeTo3DSprite(RayPals3DSprite* sprite, RayPals3DShape* shape) {
    if (!sprite || !shape) return;
    
    // Resize the array if needed
    RayPals3DShape** newShapes = (RayPals3DShape**)realloc(sprite->shapes, 
                                  sizeof(RayPals3DShape*) * (sprite->shapeCount + 1));
    if (newShapes == NULL) return;
    
    sprite->shapes = newShapes;
    sprite->shapes[sprite->shapeCount] = shape;
    sprite->shapeCount++;
}

void Draw3DSprite(RayPals3DSprite* sprite, Camera camera) {
    if (!sprite || !sprite->visible) return;
    
    // Save current matrix
    rlPushMatrix();
    
    // Apply sprite transformations
    rlTranslatef(sprite->position.x, sprite->position.y, sprite->position.z);
    
    // Apply rotations in order: Y (yaw), X (pitch), Z (roll)
    rlRotatef(sprite->rotation.y, 0.0f, 1.0f, 0.0f);
    rlRotatef(sprite->rotation.x, 1.0f, 0.0f, 0.0f);
    rlRotatef(sprite->rotation.z, 0.0f, 0.0f, 1.0f);
    
    rlScalef(sprite->scale.x, sprite->scale.y, sprite->scale.z);
    
    // Draw all shapes in the sprite RELATIVE to the sprite's transform
    // We assume Draw3DShape applies its own relative transforms correctly
    // and doesn't need the camera parameter when called this way.
    for (int i = 0; i < sprite->shapeCount; i++) {
        // Pass a dummy camera or modify Draw3DShape to handle NULL?
        // For now, let's assume Draw3DShape can handle drawing within the current matrix state.
        // We will modify Draw3DShape if needed. Let's try calling it without the camera.
        // NOTE: This will likely require modifying Draw3DShape signature or implementation.
        Draw3DShape(sprite->shapes[i], NULL); // Removed camera parameter
    }
    
    // Restore matrix
    rlPopMatrix();
}

void Set3DSpritePosition(RayPals3DSprite* sprite, Vector3 position) {
    if (sprite) sprite->position = position;
}

void Set3DSpriteRotation(RayPals3DSprite* sprite, Vector3 rotation) {
    if (sprite) sprite->rotation = rotation;
}

void Set3DSpriteScale(RayPals3DSprite* sprite, Vector3 scale) {
    if (sprite) sprite->scale = scale;
}

void Rotate3DSprite(RayPals3DSprite* sprite, float deltaTime, Vector3 speed) {
    if (!sprite) return;
    
    sprite->rotation.x += speed.x * deltaTime;
    sprite->rotation.y += speed.y * deltaTime;
    sprite->rotation.z += speed.z * deltaTime;
    
    // Normalize rotations to 0-360 degrees
    while (sprite->rotation.x >= 360.0f) sprite->rotation.x -= 360.0f;
    while (sprite->rotation.x < 0.0f) sprite->rotation.x += 360.0f;
    
    while (sprite->rotation.y >= 360.0f) sprite->rotation.y -= 360.0f;
    while (sprite->rotation.y < 0.0f) sprite->rotation.y += 360.0f;
    
    while (sprite->rotation.z >= 360.0f) sprite->rotation.z -= 360.0f;
    while (sprite->rotation.z < 0.0f) sprite->rotation.z += 360.0f;
}

void Free3DSprite(RayPals3DSprite* sprite) {
    if (!sprite) return;
    
    // Free all shapes in the sprite
    for (int i = 0; i < sprite->shapeCount; i++) {
        Free3DShape(sprite->shapes[i]);
    }
    
    // Free the shapes array and the sprite itself
    free(sprite->shapes);
    free(sprite);
}

RayPals3DSprite* Create3DRobot(Vector3 position, float size, Color bodyColor, Color detailColor) {
    RayPals3DSprite* sprite = Create3DSprite(6);
    if (sprite == NULL) return NULL;
    
    // Robot body (main cube)
    RayPals3DShape* body = CreateCube(
        (Vector3){ 0, 0, 0 },
        (Vector3){ size * 0.6f, size * 0.8f, size * 0.4f },
        bodyColor
    );
    
    // Robot head (smaller cube)
    RayPals3DShape* head = CreateCube(
        (Vector3){ 0, size * 0.6f, 0 },
        (Vector3){ size * 0.4f, size * 0.4f, size * 0.4f },
        bodyColor
    );
    
    // Robot eyes (two small cylinders)
    RayPals3DShape* leftEye = CreateCylinder(
        (Vector3){ -size * 0.1f, size * 0.6f, size * 0.2f },
        size * 0.05f,
        size * 0.05f,
        8,
        detailColor
    );
    
    RayPals3DShape* rightEye = CreateCylinder(
        (Vector3){ size * 0.1f, size * 0.6f, size * 0.2f },
        size * 0.05f,
        size * 0.05f,
        8,
        detailColor
    );
    
    // Robot arms (cylinders)
    RayPals3DShape* leftArm = CreateCylinder(
        (Vector3){ -size * 0.4f, 0, 0 },
        size * 0.1f,
        size * 0.6f,
        8,
        bodyColor
    );
    leftArm->rotation = (Vector3){ 0, 0, 90 };
    
    RayPals3DShape* rightArm = CreateCylinder(
        (Vector3){ size * 0.4f, 0, 0 },
        size * 0.1f,
        size * 0.6f,
        8,
        bodyColor
    );
    rightArm->rotation = (Vector3){ 0, 0, -90 };
    
    // Add all shapes to the sprite
    AddShapeTo3DSprite(sprite, body);
    AddShapeTo3DSprite(sprite, head);
    AddShapeTo3DSprite(sprite, leftEye);
    AddShapeTo3DSprite(sprite, rightEye);
    AddShapeTo3DSprite(sprite, leftArm);
    AddShapeTo3DSprite(sprite, rightArm);
    
    // Set sprite position
    Set3DSpritePosition(sprite, position);
    
    return sprite;
}

RayPals3DSprite* Create3DSpaceship(Vector3 position, float size, Color bodyColor, Color glassColor) {
    RayPals3DSprite* sprite = Create3DSprite(5);
    if (sprite == NULL) return NULL;
    
    // Main body (elongated cube)
    RayPals3DShape* body = CreateCube(
        (Vector3){ 0, 0, 0 },
        (Vector3){ size * 1.5f, size * 0.3f, size * 0.8f },
        bodyColor
    );
    
    // Cockpit (sphere)
    RayPals3DShape* cockpit = CreateSphere(
        (Vector3){ size * 0.4f, size * 0.2f, 0 },
        size * 0.2f,
        16,
        glassColor
    );
    
    // Wings (two flat cubes)
    RayPals3DShape* leftWing = CreateCube(
        (Vector3){ 0, 0, -size * 0.6f },
        (Vector3){ size * 0.8f, size * 0.1f, size * 0.4f },
        bodyColor
    );
    
    RayPals3DShape* rightWing = CreateCube(
        (Vector3){ 0, 0, size * 0.6f },
        (Vector3){ size * 0.8f, size * 0.1f, size * 0.4f },
        bodyColor
    );
    
    // Engine (cylinder)
    RayPals3DShape* engine = CreateCylinder(
        (Vector3){ -size * 0.6f, 0, 0 },
        size * 0.2f,
        size * 0.3f,
        12,
        bodyColor
    );
    engine->rotation = (Vector3){ 0, 0, 90 };
    
    // Add all shapes to the sprite
    AddShapeTo3DSprite(sprite, body);
    AddShapeTo3DSprite(sprite, cockpit);
    AddShapeTo3DSprite(sprite, leftWing);
    AddShapeTo3DSprite(sprite, rightWing);
    AddShapeTo3DSprite(sprite, engine);
    
    // Set sprite position
    Set3DSpritePosition(sprite, position);
    
    return sprite;
}

RayPals3DTree Create3DTree(Vector3 position, float scale, Color trunkColor, Color leavesColor) {
    RayPals3DTree tree = { 0 };
    tree.position = position;
    tree.rotation = (Vector3){ 0.0f, 0.0f, 0.0f };
    tree.scale = scale;
    
    // Create a 3D sprite with 2 shapes
    tree.sprite = Create3DSprite(2);
    if (!tree.sprite) return tree;
    
    // Set sprite position and scale
    Set3DSpritePosition(tree.sprite, position);
    Set3DSpriteScale(tree.sprite, (Vector3){ 1.0f, 1.0f, 1.0f });

    // Create trunk (cylinder)
    RayPals3DShape* trunk = CreateCylinder(
        (Vector3){ 0, scale, 0 },  // Relative to tree position
        0.3f * scale,
        2.0f * scale,
        12,
        trunkColor
    );
    
    // Create leaves (cone)
    RayPals3DShape* leaves = CreateCone(
        (Vector3){ 0, 3.0f * scale, 0 },  // Relative to tree position
        1.5f * scale,
        2.5f * scale,
        12,
        leavesColor
    );
    
    // Add shapes to sprite
    AddShapeTo3DSprite(tree.sprite, trunk);
    AddShapeTo3DSprite(tree.sprite, leaves);

    return tree;
}

void Free3DTree(RayPals3DTree* tree) {
    if (tree == NULL) return;
    
    Free3DSprite(tree->sprite);
    // Note: We're not freeing the tree itself as it might be stack-allocated
}

// Create a skeleton shape
RayPals2DShape* CreateSkeleton(Vector2 position, float size, Color color) {
    RayPals2DShape* shape = (RayPals2DShape*)malloc(sizeof(RayPals2DShape));
    if (shape == NULL) return NULL;
    
    shape->type = RAYPALS_SKELETON;
    shape->position = position;
    shape->size = (Vector2){ size, size * 1.5f }; // Skeletons are taller than wide
    shape->rotation = 0.0f;
    shape->color = color;
    shape->filled = true;
    shape->thickness = 2.0f;
    shape->segments = 0;
    shape->points = 0;
    shape->visible = true;
    
    return shape;
}

RayPalsSprite* CreateSkeletonSprite(Vector2 position, float size, Color boneColor) {
    RayPalsSprite* sprite = CreateSprite(8);
    if (sprite == NULL) return NULL;
    
    // Skull (circle)
    RayPals2DShape* skull = CreateCircle((Vector2){ 0, -size*0.35f }, size*0.15f, boneColor);
    skull->filled = false;
    skull->thickness = 2.0f;
    
    // Eye sockets (small circles)
    RayPals2DShape* eyeLeft = CreateCircle((Vector2){ -size*0.06f, -size*0.36f }, size*0.03f, BLACK);
    RayPals2DShape* eyeRight = CreateCircle((Vector2){ size*0.06f, -size*0.36f }, size*0.03f, BLACK);
    
    // Body (rectangle)
    RayPals2DShape* spine = CreateRectangle((Vector2){ 0, 0 }, (Vector2){ size*0.05f, size*0.5f }, boneColor);
    spine->filled = false;
    spine->thickness = 2.0f;
    
    // Ribcage (horizontal lines)
    RayPals2DShape* ribcage = CreateRectangle((Vector2){ 0, -size*0.15f }, (Vector2){ size*0.3f, size*0.25f }, boneColor);
    ribcage->filled = false;
    ribcage->thickness = 2.0f;
    
    // Arms
    RayPals2DShape* armLeft = CreateRectangle((Vector2){ -size*0.2f, -size*0.1f }, (Vector2){ size*0.2f, size*0.05f }, boneColor);
    armLeft->filled = false;
    armLeft->thickness = 2.0f;
    armLeft->rotation = 15.0f;
    
    RayPals2DShape* armRight = CreateRectangle((Vector2){ size*0.2f, -size*0.1f }, (Vector2){ size*0.2f, size*0.05f }, boneColor);
    armRight->filled = false;
    armRight->thickness = 2.0f;
    armRight->rotation = -15.0f;
    
    // Legs
    RayPals2DShape* legLeft = CreateRectangle((Vector2){ -size*0.1f, size*0.3f }, (Vector2){ size*0.05f, size*0.3f }, boneColor);
    legLeft->filled = false;
    legLeft->thickness = 2.0f;
    legLeft->rotation = -15.0f;
    
    RayPals2DShape* legRight = CreateRectangle((Vector2){ size*0.1f, size*0.3f }, (Vector2){ size*0.05f, size*0.3f }, boneColor);
    legRight->filled = false;
    legRight->thickness = 2.0f;
    legRight->rotation = 15.0f;
    
    // Add shapes to sprite
    AddShapeToSprite(sprite, skull);
    AddShapeToSprite(sprite, eyeLeft);
    AddShapeToSprite(sprite, eyeRight);
    AddShapeToSprite(sprite, spine);
    AddShapeToSprite(sprite, ribcage);
    AddShapeToSprite(sprite, armLeft);
    AddShapeToSprite(sprite, armRight);
    AddShapeToSprite(sprite, legLeft);
    AddShapeToSprite(sprite, legRight);
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateWaterfallSprite(Vector2 position, float width, float height, Color color) {
    const int maxDrops = 30; // Maximum number of water drops
    RayPalsSprite* sprite = CreateSprite(maxDrops);
    if (sprite == NULL) return NULL;
    
    // Calculate number of drops per row and spacing
    int dropsPerRow = 5;
    float dropSize = width / (dropsPerRow * 1.5f);
    float horizontalSpacing = width / dropsPerRow;
    float verticalSpacing = dropSize * 1.2f;
    int rows = (int)(height / verticalSpacing) + 1;
    
    // Add randomness to positions for natural look
    srand(time(NULL));
    
    // Create multiple drops in a waterfall pattern
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < dropsPerRow; col++) {
            // Add some randomness to positioning
            float offsetX = ((float)rand() / RAND_MAX - 0.5f) * (horizontalSpacing * 0.4f);
            float offsetY = ((float)rand() / RAND_MAX - 0.5f) * (verticalSpacing * 0.4f);
            
            // Calculate position of this drop
            Vector2 dropPos = {
                -width/2 + col * horizontalSpacing + horizontalSpacing/2 + offsetX,
                -height/2 + row * verticalSpacing + offsetY
            };
            
            // Create smaller drops higher up, larger drops lower down
            float sizeVariation = 0.6f + 0.4f * (float)row / rows;
            float thisDropSize = dropSize * sizeVariation;
            
            // Vary color slightly for visual interest
            Color dropColor = color;
            dropColor.a = (unsigned char)(200 + ((float)rand() / RAND_MAX) * 55);
            
            // Create the water drop and add it to the sprite
            RayPals2DShape* drop = CreateWaterDrop(dropPos, thisDropSize, 0.0f, dropColor);
            AddShapeToSprite(sprite, drop);
        }
    }
    
    // Set sprite position
    sprite->position = position;
    
    return sprite;
}

RayPalsSprite* CreateApple(Vector2 position, float size, Color color, Color stemColor) {
    RayPalsSprite* sprite = CreateSprite(3);
    if (!sprite) return NULL;

    // Main apple body (circle)
    RayPals2DShape* body = CreateCircle((Vector2){0, 0}, size, color);
    AddShapeToSprite(sprite, body);

    // Stem (rectangle)
    RayPals2DShape* stem = CreateRectangle((Vector2){0, -size/2}, (Vector2){size/8, size/3}, stemColor);
    AddShapeToSprite(sprite, stem);

    // Leaf (triangle)
    RayPals2DShape* leaf = CreateTriangle((Vector2){size/4, -size/2}, size/4, stemColor);
    SetShapeRotation(leaf, 45.0f);
    AddShapeToSprite(sprite, leaf);

    SetSpritePosition(sprite, position);
    return sprite;
}

RayPalsSprite* CreateBanana(Vector2 position, float size, Color color, Color stemColor) {
    RayPalsSprite* sprite = CreateSprite(3);
    if (!sprite) return NULL;

    // Main banana body (rectangle with rounded ends)
    RayPals2DShape* body = CreateRectangle((Vector2){0, 0}, (Vector2){size, size/3}, color);
    AddShapeToSprite(sprite, body);

    // Stem (rectangle)
    RayPals2DShape* stem = CreateRectangle((Vector2){-size/2, -size/6}, (Vector2){size/8, size/4}, stemColor);
    AddShapeToSprite(sprite, stem);

    // Curve (triangle for banana curve)
    RayPals2DShape* curve = CreateTriangle((Vector2){size/4, 0}, size/4, color);
    SetShapeRotation(curve, 30.0f);
    AddShapeToSprite(sprite, curve);

    SetSpritePosition(sprite, position);
    return sprite;
}

RayPalsSprite* CreateOrange(Vector2 position, float size, Color color, Color stemColor) {
    RayPalsSprite* sprite = CreateSprite(3);
    if (!sprite) return NULL;

    // Main orange body (circle)
    RayPals2DShape* body = CreateCircle((Vector2){0, 0}, size, color);
    AddShapeToSprite(sprite, body);

    // Stem (rectangle)
    RayPals2DShape* stem = CreateRectangle((Vector2){0, -size/2}, (Vector2){size/8, size/4}, stemColor);
    AddShapeToSprite(sprite, stem);

    // Leaf (triangle)
    RayPals2DShape* leaf = CreateTriangle((Vector2){size/4, -size/2}, size/4, stemColor);
    SetShapeRotation(leaf, 45.0f);
    AddShapeToSprite(sprite, leaf);

    SetSpritePosition(sprite, position);
    return sprite;
}

RayPalsSprite* CreateWatermelon(Vector2 position, float size, Color rindColor, Color fleshColor, Color seedColor) {
    RayPalsSprite* sprite = CreateSprite(5);
    if (!sprite) return NULL;

    // Main watermelon body (circle)
    RayPals2DShape* body = CreateCircle((Vector2){0, 0}, size, rindColor);
    AddShapeToSprite(sprite, body);

    // Flesh (slightly smaller circle)
    RayPals2DShape* flesh = CreateCircle((Vector2){0, 0}, size * 0.9f, fleshColor);
    AddShapeToSprite(sprite, flesh);

    // Seeds (small circles)
    RayPals2DShape* seed1 = CreateCircle((Vector2){size/4, size/4}, size/8, seedColor);
    RayPals2DShape* seed2 = CreateCircle((Vector2){-size/4, -size/4}, size/8, seedColor);
    RayPals2DShape* seed3 = CreateCircle((Vector2){size/4, -size/4}, size/8, seedColor);
    AddShapeToSprite(sprite, seed1);
    AddShapeToSprite(sprite, seed2);
    AddShapeToSprite(sprite, seed3);

    SetSpritePosition(sprite, position);
    return sprite;
}

RayPalsSprite* CreateGrape(Vector2 position, float size, Color color, Color stemColor) {
    RayPalsSprite* sprite = CreateSprite(3);
    if (!sprite) return NULL;

    // Main grape body (circle)
    RayPals2DShape* body = CreateCircle((Vector2){0, 0}, size, color);
    AddShapeToSprite(sprite, body);

    // Stem (rectangle)
    RayPals2DShape* stem = CreateRectangle((Vector2){0, -size/2}, (Vector2){size/8, size/3}, stemColor);
    AddShapeToSprite(sprite, stem);

    // Highlight (small circle)
    RayPals2DShape* highlight = CreateCircle((Vector2){-size/4, -size/4}, size/4, (Color){255, 255, 255, 128});
    AddShapeToSprite(sprite, highlight);

    SetSpritePosition(sprite, position);
    return sprite;
}

RayPalsSprite* CreateGrapes(Vector2 position, float size, Color color, Color stemColor) {
    RayPalsSprite* sprite = CreateSprite(9);  // Capacity for main stem and 8 grapes
    if (!sprite) return NULL;

    // Main stem (rectangle)
    RayPals2DShape* stem = CreateRectangle((Vector2){0, -size/2}, (Vector2){size/8, size/3}, stemColor);
    AddShapeToSprite(sprite, stem);

    // Create individual grapes (just the body and highlight, no stems)
    // Top row
    RayPals2DShape* grape1 = CreateCircle((Vector2){-size/4, -size/4}, size/4, color);
    RayPals2DShape* grape2 = CreateCircle((Vector2){0, -size/4}, size/4, color);
    RayPals2DShape* grape3 = CreateCircle((Vector2){size/4, -size/4}, size/4, color);
    
    // Middle row
    RayPals2DShape* grape4 = CreateCircle((Vector2){-size/3, 0}, size/4, color);
    RayPals2DShape* grape5 = CreateCircle((Vector2){0, 0}, size/4, color);
    RayPals2DShape* grape6 = CreateCircle((Vector2){size/3, 0}, size/4, color);
    
    // Bottom row
    RayPals2DShape* grape7 = CreateCircle((Vector2){-size/4, size/4}, size/4, color);
    RayPals2DShape* grape8 = CreateCircle((Vector2){size/4, size/4}, size/4, color);

    // Add highlights to each grape
    RayPals2DShape* highlight1 = CreateCircle((Vector2){-size/4 - size/8, -size/4 - size/8}, size/8, (Color){255, 255, 255, 128});
    RayPals2DShape* highlight2 = CreateCircle((Vector2){-size/8, -size/4 - size/8}, size/8, (Color){255, 255, 255, 128});
    RayPals2DShape* highlight3 = CreateCircle((Vector2){size/4 - size/8, -size/4 - size/8}, size/8, (Color){255, 255, 255, 128});
    RayPals2DShape* highlight4 = CreateCircle((Vector2){-size/3 - size/8, -size/8}, size/8, (Color){255, 255, 255, 128});
    RayPals2DShape* highlight5 = CreateCircle((Vector2){-size/8, -size/8}, size/8, (Color){255, 255, 255, 128});
    RayPals2DShape* highlight6 = CreateCircle((Vector2){size/3 - size/8, -size/8}, size/8, (Color){255, 255, 255, 128});
    RayPals2DShape* highlight7 = CreateCircle((Vector2){-size/4 - size/8, size/4 - size/8}, size/8, (Color){255, 255, 255, 128});
    RayPals2DShape* highlight8 = CreateCircle((Vector2){size/4 - size/8, size/4 - size/8}, size/8, (Color){255, 255, 255, 128});

    // Add all grapes and highlights to the sprite
    AddShapeToSprite(sprite, grape1);
    AddShapeToSprite(sprite, grape2);
    AddShapeToSprite(sprite, grape3);
    AddShapeToSprite(sprite, grape4);
    AddShapeToSprite(sprite, grape5);
    AddShapeToSprite(sprite, grape6);
    AddShapeToSprite(sprite, grape7);
    AddShapeToSprite(sprite, grape8);
    AddShapeToSprite(sprite, highlight1);
    AddShapeToSprite(sprite, highlight2);
    AddShapeToSprite(sprite, highlight3);
    AddShapeToSprite(sprite, highlight4);
    AddShapeToSprite(sprite, highlight5);
    AddShapeToSprite(sprite, highlight6);
    AddShapeToSprite(sprite, highlight7);
    AddShapeToSprite(sprite, highlight8);

    SetSpritePosition(sprite, position);
    return sprite;
}