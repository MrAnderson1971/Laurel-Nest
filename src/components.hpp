#pragma once
#include "common.hpp"
#define STB_IMAGE_IMPLEMENTATION

struct TransformComponent {
    glm::vec3 position;
    glm::vec3 scale;
    float rotation;
};

struct SpriteComponent {
    GLuint textureID;
    float width;
    float height;
};

// Player component
struct Player
{

};

// anything that is deadly to the player
struct Deadly
{

};

// A timer that will be associated to dying salmon
struct DeathTimer
{
    float counter_ms = 3000;
};

struct Motion {
    vec2 position = { 0, 0 };
    float angle = 0;
    vec2 velocity = { 0, 0 };
    vec2 scale = { 10, 10 };
};

struct Collision
{
    // Note, the first object is stored in the ECS container.entities
    Entity other; // the second object involved in the collision
    Collision(Entity& other) { this->other = other; };
};

// Mesh data structure for storing vertex and index buffers
struct Mesh
{
   // static bool loadFromOBJFile(std::string obj_path, std::vector<ColoredVertex>& out_vertices, std::vector<uint16_t>& out_vertex_indices, vec2& out_size);
    vec2 original_size = {1,1};
    // std::vector<ColoredVertex> vertices;
    // std::vector<uint16_t> vertex_indices;
};

// Data structure for toggling debug mode
struct Debug {
    bool in_debug_mode = 0;
    bool in_freeze_mode = 0;
};
extern Debug debugging;

// Sets the brightness of the screen
struct ScreenState
{
    float darken_screen_factor = -1;
};

// A struct to refer to debugging graphics in the ECS
struct DebugComponent
{
    // Note, an empty struct has size 1
};

enum class TEXTURE_ASSET_ID {
    PLAYER_WALK = 0,
    PLAYER_JUMP = PLAYER_WALK + 1,
    PLAYER_ATTACK = PLAYER_JUMP + 1,
    TEXTURE_COUNT = PLAYER_ATTACK + 1
};
const int texture_count = (int)TEXTURE_ASSET_ID::TEXTURE_COUNT;

enum class EFFECT_ASSET_ID {
    PLAYER_EFFECT = 0,
    EFFECT_COUNT = PLAYER_EFFECT + 1
};
const int effect_count = (int)EFFECT_ASSET_ID::EFFECT_COUNT;

enum class GEOMETRY_BUFFER_ID {
    PLAYER_GEO = 0,
    SPRITE = PLAYER_GEO + 1,
    GEOMETRY_COUNT = SPRITE + 1
};
const int geometry_count = (int)GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;

struct RenderRequest {
    TEXTURE_ASSET_ID used_texture = TEXTURE_ASSET_ID::TEXTURE_COUNT;
    EFFECT_ASSET_ID used_effect = EFFECT_ASSET_ID::EFFECT_COUNT;
    GEOMETRY_BUFFER_ID used_geometry = GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;
=======
struct MotionComponent {
    vec2 position;
    vec2 velocity;
    vec2 acceleration;
    float angle;
};

struct AffectedByGravityComponent {

};

struct BreakableDoorComponent {
    bool broken = false;
};

struct FlamethrowerComponent {

};

struct PlayerComponent {

};
