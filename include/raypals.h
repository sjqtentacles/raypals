/**
 * @file raypals.h
 * @brief RayPals - A simple 2D/3D shape and sprite library built on raylib
 * 
 * RayPals provides an easy-to-use interface for creating and manipulating 2D and 3D shapes,
 * as well as composing them into sprites for game development. It builds on top of raylib
 * to provide a higher-level abstraction while maintaining simplicity and flexibility.
 * 
 * Key features:
 * - Basic 2D shapes (square, circle, triangle, etc.)
 * - Basic 3D shapes (cube, sphere, cone, etc.)
 * - Complex shapes (polygon, arrow)
 * - Sprite system for combining shapes
 * - Pre-built character and environment sprites
 * - Animation support
 * 
 * @version 1.0.0
 * @date 2024
 * @license MIT
 */

#ifndef RAYPALS_H
#define RAYPALS_H

#include <raylib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enumeration of available shape types
 * 
 * This enum defines all the basic and complex shapes that can be created
 * using the RayPals library. Each shape type has specific properties and
 * drawing methods.
 */
typedef enum {
    RAYPALS_SQUARE,     ///< A square shape
    RAYPALS_RECTANGLE,  ///< A rectangle shape
    RAYPALS_CIRCLE,     ///< A circle shape
    RAYPALS_TRIANGLE,   ///< A triangle shape
    RAYPALS_STAR,       ///< A star shape with configurable points
    RAYPALS_CUBE,       ///< A 3D cube
    RAYPALS_SPHERE,     ///< A 3D sphere
    RAYPALS_CONE,       ///< A 3D cone
    RAYPALS_CYLINDER,   ///< A 3D cylinder
    RAYPALS_TREE,       ///< A simple tree shape
    RAYPALS_SKELETON,   ///< A skeleton shape
    RAYPALS_POLYGON,    ///< A regular polygon with configurable sides
    RAYPALS_ARROW       ///< An arrow shape with configurable direction
} RayPalsShapeType;

/**
 * @brief Structure representing a 2D shape
 * 
 * This structure contains all the properties needed to define and draw a 2D shape.
 * It includes position, size, rotation, color, and other visual properties.
 */
typedef struct {
    RayPalsShapeType type;      ///< The type of shape
    Vector2 position;           ///< Position in 2D space
    Vector2 size;              ///< Size of the shape
    float rotation;            ///< Rotation in degrees
    Color color;               ///< Color of the shape
    bool filled;               ///< Whether the shape is filled or outlined
    float thickness;           ///< Line thickness for outlined shapes
    int segments;              ///< Number of segments for circle/star
    int points;                ///< Number of points for star
    bool visible;              ///< Whether the shape is visible
} RayPals2DShape;

/**
 * @brief Structure representing a 3D shape
 * 
 * This structure contains all the properties needed to define and draw a 3D shape.
 * It includes position, size, rotation, color, and other visual properties.
 */
typedef struct {
    RayPalsShapeType type;      ///< The type of shape
    Vector3 position;           ///< Position in 3D space
    Vector3 size;              ///< Size of the shape
    Vector3 rotation;          ///< Rotation in degrees (x, y, z)
    Color color;               ///< Color of the shape
    bool wireframe;            ///< Whether to draw as wireframe
    float thickness;           ///< Line thickness for wireframe
    int segments;              ///< Number of segments for sphere/cone/cylinder
    bool visible;              ///< Whether the shape is visible
} RayPals3DShape;

/**
 * @brief Structure for animated shape properties
 * 
 * This structure defines animation properties that can be applied to shapes,
 * including scaling, rotation, and color transitions.
 */
typedef struct {
    bool isAnimated;           ///< Whether the shape is animated
    float animationSpeed;      ///< Speed of the animation
    float animationTime;       ///< Current animation time
    float scaleMin;            ///< Minimum scale factor
    float scaleMax;            ///< Maximum scale factor
    float rotationSpeed;       ///< Rotation speed in degrees per second
    Color colorStart;          ///< Starting color for color transitions
    Color colorEnd;            ///< Ending color for color transitions
    bool pingPong;             ///< Whether to reverse animation at endpoints
    float originalWidth;       ///< Original width of the shape (for maintaining proper scaling)
    float originalHeight;      ///< Original height of the shape (for maintaining proper scaling)
    float originalDepth;       ///< Original depth of the shape (for 3D shapes)
} RayPalsAnimation;

/**
 * @brief Structure representing a sprite (collection of shapes)
 * 
 * A sprite is a collection of 2D shapes that can be manipulated as a single unit.
 * It supports position, rotation, and scale transformations that affect all shapes.
 */
typedef struct {
    RayPals2DShape** shapes;   ///< Array of pointers to shapes
    int shapeCount;            ///< Number of shapes in the sprite
    Vector2 position;          ///< Master position
    float rotation;            ///< Master rotation
    float scale;               ///< Master scale factor
    bool visible;              ///< Visibility flag
} RayPalsSprite;

/**
 * @brief Creates a square shape
 * 
 * @param position The center position of the square
 * @param size The size of the square (width and height)
 * @param color The color of the square
 * @return A pointer to the created square shape
 */
RayPals2DShape* CreateSquare(Vector2 position, float size, Color color);

/**
 * @brief Creates a rectangle shape
 * 
 * @param position The center position of the rectangle
 * @param size The size of the rectangle (width and height)
 * @param color The color of the rectangle
 * @return A pointer to the created rectangle shape
 */
RayPals2DShape* CreateRectangle(Vector2 position, Vector2 size, Color color);

/**
 * @brief Creates a circle shape
 * 
 * @param position The center position of the circle
 * @param radius The radius of the circle
 * @param color The color of the circle
 * @return A pointer to the created circle shape
 */
RayPals2DShape* CreateCircle(Vector2 position, float radius, Color color);

/**
 * @brief Creates a triangle shape
 * 
 * @param position The center position of the triangle
 * @param size The size of the triangle
 * @param color The color of the triangle
 * @return A pointer to the created triangle shape
 */
RayPals2DShape* CreateTriangle(Vector2 position, float size, Color color);

/**
 * @brief Creates a star shape
 * 
 * @param position The center position of the star
 * @param size The size of the star
 * @param points The number of points on the star
 * @param color The color of the star
 * @return A pointer to the created star shape
 */
RayPals2DShape* CreateStar(Vector2 position, float size, int points, Color color);

/**
 * @brief Creates a regular polygon shape
 * 
 * @param position The center position of the polygon
 * @param radius The radius of the polygon
 * @param sides The number of sides of the polygon
 * @param color The color of the polygon
 * @return A pointer to the created polygon shape
 */
RayPals2DShape* CreatePolygon(Vector2 position, float radius, int sides, Color color);

/**
 * @brief Creates an arrow shape
 * 
 * @param position The center position of the arrow
 * @param size The size of the arrow
 * @param direction The direction of the arrow in degrees
 * @param color The color of the arrow
 * @return A pointer to the created arrow shape
 */
RayPals2DShape* CreateArrow(Vector2 position, float size, float direction, Color color);

/**
 * @brief Creates a 3D cube shape
 * 
 * @param position The center position of the cube
 * @param size The size of the cube (width, height, depth)
 * @param color The color of the cube
 * @return A pointer to the created cube shape
 */
RayPals3DShape* CreateCube(Vector3 position, Vector3 size, Color color);

/**
 * @brief Creates a 3D sphere shape
 * 
 * @param position The center position of the sphere
 * @param radius The radius of the sphere
 * @param segments The number of segments for the sphere
 * @param color The color of the sphere
 * @return A pointer to the created sphere shape
 */
RayPals3DShape* CreateSphere(Vector3 position, float radius, int segments, Color color);

/**
 * @brief Creates a 3D cone shape
 * 
 * @param position The center position of the cone
 * @param radius The radius of the cone base
 * @param height The height of the cone
 * @param segments The number of segments for the cone
 * @param color The color of the cone
 * @return A pointer to the created cone shape
 */
RayPals3DShape* CreateCone(Vector3 position, float radius, float height, int segments, Color color);

/**
 * @brief Creates a 3D cylinder shape
 * 
 * @param position The center position of the cylinder
 * @param radius The radius of the cylinder
 * @param height The height of the cylinder
 * @param segments The number of segments for the cylinder
 * @param color The color of the cylinder
 * @return A pointer to the created cylinder shape
 */
RayPals3DShape* CreateCylinder(Vector3 position, float radius, float height, int segments, Color color);

/**
 * @brief Creates a new sprite with the specified initial capacity
 * 
 * @param initialCapacity The initial number of shapes the sprite can hold
 * @return A pointer to the created sprite
 */
RayPalsSprite* CreateSprite(int initialCapacity);

/**
 * @brief Adds a shape to a sprite
 * 
 * @param sprite The sprite to add the shape to
 * @param shape The shape to add
 */
void AddShapeToSprite(RayPalsSprite* sprite, RayPals2DShape* shape);

/**
 * @brief Creates a simple character sprite
 * 
 * @param position The position of the character
 * @param size The size of the character
 * @param bodyColor The color of the character's body
 * @param headColor The color of the character's head
 * @return A pointer to the created character sprite
 */
RayPalsSprite* CreateSimpleCharacter(Vector2 position, float size, Color bodyColor, Color headColor);

/**
 * @brief Creates a robot character sprite
 * 
 * @param position The position of the robot
 * @param size The size of the robot
 * @param primaryColor The primary color of the robot
 * @param secondaryColor The secondary color of the robot
 * @return A pointer to the created robot sprite
 */
RayPalsSprite* CreateRobotCharacter(Vector2 position, float size, Color primaryColor, Color secondaryColor);

/**
 * @brief Creates an animal character sprite
 * 
 * @param position The position of the animal
 * @param size The size of the animal
 * @param bodyColor The color of the animal's body
 * @param detailColor The color of the animal's details
 * @return A pointer to the created animal sprite
 */
RayPalsSprite* CreateAnimalCharacter(Vector2 position, float size, Color bodyColor, Color detailColor);

/**
 * @brief Creates a ghost sprite
 * 
 * @param position The position of the ghost
 * @param size The size of the ghost
 * @param color The color of the ghost
 * @return A pointer to the created ghost sprite
 */
RayPalsSprite* CreateGhost(Vector2 position, float size, Color color);

/**
 * @brief Creates a simple tree sprite
 * 
 * @param position The position of the tree
 * @param size The size of the tree
 * @param trunkColor The color of the tree trunk
 * @param leafColor The color of the tree leaves
 * @return A pointer to the created tree sprite
 */
RayPalsSprite* CreateSimpleTree(Vector2 position, float size, Color trunkColor, Color leafColor);

/**
 * @brief Creates a cloud sprite
 * 
 * @param position The position of the cloud
 * @param size The size of the cloud
 * @param color The color of the cloud
 * @return A pointer to the created cloud sprite
 */
RayPalsSprite* CreateCloud(Vector2 position, float size, Color color);

/**
 * @brief Creates a house sprite
 * 
 * @param position The position of the house
 * @param size The size of the house
 * @param wallColor The color of the house walls
 * @param roofColor The color of the house roof
 * @return A pointer to the created house sprite
 */
RayPalsSprite* CreateHouse(Vector2 position, float size, Color wallColor, Color roofColor);

/**
 * @brief Creates a bush sprite
 * 
 * @param position The position of the bush
 * @param size The size of the bush
 * @param color The color of the bush
 * @return A pointer to the created bush sprite
 */
RayPalsSprite* CreateBush(Vector2 position, float size, Color color);

/**
 * @brief Creates a rock sprite
 * 
 * @param position The position of the rock
 * @param size The size of the rock
 * @param color The color of the rock
 * @return A pointer to the created rock sprite
 */
RayPalsSprite* CreateRock(Vector2 position, float size, Color color);

/**
 * @brief Creates a button sprite
 * 
 * @param position The position of the button
 * @param size The size of the button
 * @param bgColor The background color of the button
 * @param borderColor The color of the button border
 * @return A pointer to the created button sprite
 */
RayPalsSprite* CreateButton(Vector2 position, Vector2 size, Color bgColor, Color borderColor);

/**
 * @brief Creates a health bar sprite
 * 
 * @param position The position of the health bar
 * @param width The width of the health bar
 * @param fillPercent The percentage of the health bar that is filled (0.0 to 1.0)
 * @param emptyColor The color of the empty portion
 * @param fillColor The color of the filled portion
 * @return A pointer to the created health bar sprite
 */
RayPalsSprite* CreateHealthBar(Vector2 position, float width, float fillPercent, Color emptyColor, Color fillColor);

/**
 * @brief Creates a car sprite
 * 
 * @param position The center position of the car
 * @param size The size scale of the car
 * @param bodyColor The main color of the car body
 * @param detailColor The color for wheels and details
 * @return A pointer to the created car sprite
 */
RayPalsSprite* CreateCar(Vector2 position, float size, Color bodyColor, Color detailColor);

/**
 * @brief Creates a tank sprite
 * 
 * @param position The center position of the tank
 * @param size The size scale of the tank
 * @param bodyColor The main color of the tank body
 * @param detailColor The color for treads and details
 * @return A pointer to the created tank sprite
 */
RayPalsSprite* CreateTank(Vector2 position, float size, Color bodyColor, Color detailColor);

/**
 * @brief Creates a motorcycle sprite
 * 
 * @param position The center position of the motorcycle
 * @param size The size scale of the motorcycle
 * @param bodyColor The main color of the motorcycle body
 * @param detailColor The color for wheels and details
 * @return A pointer to the created motorcycle sprite
 */
RayPalsSprite* CreateMotorcycle(Vector2 position, float size, Color bodyColor, Color detailColor);

/**
 * @brief Creates a skateboard sprite
 * 
 * @param position The center position of the skateboard
 * @param size The size scale of the skateboard
 * @param deckColor The color of the skateboard deck
 * @param wheelColor The color of the wheels
 * @return A pointer to the created skateboard sprite
 */
RayPalsSprite* CreateSkateboard(Vector2 position, float size, Color deckColor, Color wheelColor);

/**
 * @brief Creates a sword sprite
 * 
 * @param position The center position of the sword
 * @param size The size scale of the sword
 * @param bladeColor The color of the sword blade
 * @param hiltColor The color of the sword hilt
 * @return A pointer to the created sword sprite
 */
RayPalsSprite* CreateSword(Vector2 position, float size, Color bladeColor, Color hiltColor);

/**
 * @brief Creates an arrow sprite
 * 
 * @param position The center position of the arrow
 * @param size The size scale of the arrow
 * @param shaftColor The color of the arrow shaft
 * @param headColor The color of the arrow head
 * @return A pointer to the created arrow sprite
 */
RayPalsSprite* CreateArrowSprite(Vector2 position, float size, Color shaftColor, Color headColor);

/**
 * @brief Creates a sailboat sprite
 * 
 * @param position The center position of the sailboat
 * @param size The size scale of the sailboat
 * @param hullColor The color of the boat hull
 * @param sailColor The color of the sail
 * @return A pointer to the created sailboat sprite
 */
RayPalsSprite* CreateSailboat(Vector2 position, float size, Color hullColor, Color sailColor);

/**
 * @brief Draws a 2D shape
 * 
 * @param shape The shape to draw
 */
void Draw2DShape(RayPals2DShape* shape);

/**
 * @brief Draws a 3D shape
 * 
 * @param shape The shape to draw
 * @param camera The camera to use for 3D rendering
 */
void Draw3DShape(RayPals3DShape* shape, Camera camera);

/**
 * @brief Draws a sprite
 * 
 * @param sprite The sprite to draw
 */
void DrawSprite(RayPalsSprite* sprite);

/**
 * @brief Updates the animation of a 2D shape
 * 
 * @param shape The shape to animate
 * @param animation The animation properties
 * @param deltaTime The time elapsed since the last update
 */
void UpdateShapeAnimation(RayPals2DShape* shape, RayPalsAnimation* animation, float deltaTime);

/**
 * @brief Updates the animation of a 3D shape
 * 
 * @param shape The shape to animate
 * @param animation The animation properties
 * @param deltaTime The time elapsed since the last update
 */
void Update3DShapeAnimation(RayPals3DShape* shape, RayPalsAnimation* animation, float deltaTime);

/**
 * @brief Sets the color of a 2D shape
 * 
 * @param shape The shape to modify
 * @param color The new color
 */
void SetShapeColor(RayPals2DShape* shape, Color color);

/**
 * @brief Sets the color of a 3D shape
 * 
 * @param shape The shape to modify
 * @param color The new color
 */
void Set3DShapeColor(RayPals3DShape* shape, Color color);

/**
 * @brief Sets the rotation of a 2D shape
 * 
 * @param shape The shape to modify
 * @param rotation The new rotation in degrees
 */
void SetShapeRotation(RayPals2DShape* shape, float rotation);

/**
 * @brief Sets the rotation of a 3D shape
 * 
 * @param shape The shape to modify
 * @param rotation The new rotation in degrees (x, y, z)
 */
void Set3DShapeRotation(RayPals3DShape* shape, Vector3 rotation);

/**
 * @brief Sets the position of a 2D shape
 * 
 * @param shape The shape to modify
 * @param position The new position
 */
void SetShapePosition(RayPals2DShape* shape, Vector2 position);

/**
 * @brief Sets the position of a 3D shape
 * 
 * @param shape The shape to modify
 * @param position The new position
 */
void Set3DShapePosition(RayPals3DShape* shape, Vector3 position);

/**
 * @brief Rotates a 2D shape by a given speed
 * 
 * @param shape The shape to rotate
 * @param deltaTime The time elapsed since the last update
 * @param speed The rotation speed in degrees per second
 */
void RotateShape(RayPals2DShape* shape, float deltaTime, float speed);

/**
 * @brief Rotates a 3D shape by a given speed
 * 
 * @param shape The shape to rotate
 * @param deltaTime The time elapsed since the last update
 * @param speed The rotation speed in degrees per second (x, y, z)
 */
void Rotate3DShape(RayPals3DShape* shape, float deltaTime, Vector3 speed);

/**
 * @brief Rotates a sprite by a given speed
 * 
 * @param sprite The sprite to rotate
 * @param deltaTime The time elapsed since the last update
 * @param speed The rotation speed in degrees per second
 */
void RotateSprite(RayPalsSprite* sprite, float deltaTime, float speed);

/**
 * @brief Sets the position of a sprite
 * 
 * @param sprite The sprite to modify
 * @param position The new position
 */
void SetSpritePosition(RayPalsSprite* sprite, Vector2 position);

/**
 * @brief Sets the rotation of a sprite
 * 
 * @param sprite The sprite to modify
 * @param rotation The new rotation in degrees
 */
void SetSpriteRotation(RayPalsSprite* sprite, float rotation);

/**
 * @brief Sets the scale of a sprite
 * 
 * @param sprite The sprite to modify
 * @param scale The new scale factor
 */
void SetSpriteScale(RayPalsSprite* sprite, float scale);

/**
 * @brief Frees the memory allocated for a 2D shape
 * 
 * @param shape The shape to free
 */
void FreeShape(RayPals2DShape* shape);

/**
 * @brief Frees the memory allocated for a 3D shape
 * 
 * @param shape The shape to free
 */
void Free3DShape(RayPals3DShape* shape);

/**
 * @brief Frees the memory allocated for a sprite
 * 
 * @param sprite The sprite to free
 */
void FreeSprite(RayPalsSprite* sprite);

/**
 * @brief Creates a sprite with a star shape
 * 
 * @param position The position of the star
 * @param size The size of the star
 * @param color The color of the star
 * @return A pointer to the created star sprite
 */
RayPalsSprite* CreateStarSprite(Vector2 position, float size, Color color);

/**
 * @brief Creates a sprite with a yellow star shape
 * 
 * @param position The position of the star
 * @param size The size of the star
 * @return A pointer to the created yellow star sprite
 */
RayPalsSprite* CreateYellowStar(Vector2 position, float size);

/**
 * @brief Creates a flower sprite
 * 
 * @param position The center position of the flower
 * @param size The size scale of the flower
 * @param petalColor The color of the flower petals
 * @param centerColor The color of the flower center
 * @return A pointer to the created flower sprite
 */
RayPalsSprite* CreateFlower(Vector2 position, float size, Color petalColor, Color centerColor);

/**
 * @brief Creates a coin sprite
 * 
 * @param position The center position of the coin
 * @param size The size scale of the coin
 * @param color The color of the coin
 * @return A pointer to the created coin sprite
 */
RayPalsSprite* CreateCoin(Vector2 position, float size, Color color);

/**
 * @brief Creates a key sprite
 * 
 * @param position The center position of the key
 * @param size The size scale of the key
 * @param color The color of the key
 * @return A pointer to the created key sprite
 */
RayPalsSprite* CreateKey(Vector2 position, float size, Color color);

/**
 * @brief Creates a shield sprite
 * 
 * @param position The center position of the shield
 * @param size The size scale of the shield
 * @param mainColor The main color of the shield
 * @param borderColor The color of the shield border
 * @return A pointer to the created shield sprite
 */
RayPalsSprite* CreateShield(Vector2 position, float size, Color mainColor, Color borderColor);

/**
 * @brief Creates a crown sprite
 * 
 * @param position The center position of the crown
 * @param size The size scale of the crown
 * @param color The color of the crown
 * @param gemColor The color of the gems on the crown
 * @return A pointer to the created crown sprite
 */
RayPalsSprite* CreateCrown(Vector2 position, float size, Color color, Color gemColor);

/**
 * @brief Creates a lightning bolt sprite
 * 
 * @param position The center position of the lightning bolt
 * @param size The size scale of the lightning bolt
 * @param color The color of the lightning bolt
 * @return A pointer to the created lightning bolt sprite
 */
RayPalsSprite* CreateLightningBolt(Vector2 position, float size, Color color);

/**
 * @brief Creates a fish sprite
 * 
 * @param position The center position of the fish
 * @param size The size scale of the fish
 * @param bodyColor The color of the fish body
 * @param finColor The color of the fish fins
 * @return A pointer to the created fish sprite
 */
RayPalsSprite* CreateFish(Vector2 position, float size, Color bodyColor, Color finColor);

/**
 * @brief Creates a gem/diamond sprite
 * 
 * @param position The center position of the gem
 * @param size The size scale of the gem
 * @param color The color of the gem
 * @return A pointer to the created gem sprite
 */
RayPalsSprite* CreateGem(Vector2 position, float size, Color color);

#ifdef __cplusplus
}
#endif

#endif // RAYPALS_H