#include "raypals.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
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
        
        default: break;
    }
    
    // Restore matrix
    rlPopMatrix();
}

void Draw3DShape(RayPals3DShape* shape, Camera camera) {
    if (!shape || !shape->visible) return;
    
    // Begin 3D mode with the given camera
    BeginMode3D(camera);
    
    switch (shape->type) {
        case RAYPALS_CUBE: {
            if (shape->wireframe) {
                DrawCubeWires(shape->position, shape->size.x, shape->size.y, shape->size.z, shape->color);
            } else {
                DrawCube(shape->position, shape->size.x, shape->size.y, shape->size.z, shape->color);
            }
        } break;
        
        case RAYPALS_SPHERE: {
            float radius = shape->size.x/2;
            
            if (shape->wireframe) {
                DrawSphereWires(shape->position, radius, shape->segments, shape->segments, shape->color);
            } else {
                DrawSphere(shape->position, radius, shape->color);
            }
        } break;
        
        case RAYPALS_CONE: {
            float radius = shape->size.x/2;
            float height = shape->size.y;
            
            if (shape->wireframe) {
                DrawCylinderWires(shape->position, radius, 0.0f, height, shape->segments, shape->color);
            } else {
                DrawCylinder(shape->position, radius, 0.0f, height, shape->segments, shape->color);
            }
        } break;
        
        case RAYPALS_CYLINDER: {
            float radius = shape->size.x/2;
            float height = shape->size.y;
            
            if (shape->wireframe) {
                DrawCylinderWires(shape->position, radius, radius, height, shape->segments, shape->color);
            } else {
                DrawCylinder(shape->position, radius, radius, height, shape->segments, shape->color);
            }
        } break;
        
        default: break;
    }
    
    EndMode3D();
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
    
    // Print debug info
    printf("Created star sprite with size %.2f, color (%d,%d,%d,%d)\n", 
           size, color.r, color.g, color.b, color.a);
    
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

// Create a water drop shape - empty implementation
RayPals2DShape* CreateWaterDrop(Vector2 position, float size, float rotation, Color color) {
    // This function is intentionally left empty
    return NULL;
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
    printf("Creating YELLOW star at (%.1f, %.1f) with size %.1f\n", position.x, position.y, size);
    
    // Create a star with yellow color and increased size for visibility
    RayPalsSprite* star = CreateStarSprite(position, size * 1.2f, brightYellow);
    
    // Apply additional settings if needed
    if (star) {
        // Ensure the star is visible
        star->visible = true;
        printf("Yellow star created successfully with %d shapes\n", star->shapeCount);
    } else {
        printf("ERROR: Failed to create yellow star!\n");
    }
    
    return star;
}