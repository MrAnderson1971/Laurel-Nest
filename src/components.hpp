#pragma once
#include "common.hpp"
#include <utility>
#include <vector>
#include <unordered_map>
#include "../ext/stb_image/stb_image.h"
#include <memory>
#include <unordered_set>

struct Sprite {
    std::shared_ptr<GLuint> textureID;
    const float width = 1.0f;
    const float height = 1.0f;

    Sprite(GLuint id) {
        textureID = std::shared_ptr<GLuint>(new GLuint(id), [](GLuint* id) {
            if (glIsTexture(*id)) {
                glDeleteTextures(1, id);
            }
            delete id;
        });
    }

    Sprite& operator=(const Sprite& other) {
        textureID = other.textureID;
        return *this;
    }
};

enum PlayerState {
    IDLE,
    WALKING,
    ATTACKING,
    JUMPING,
};

/* Template Animation component for animated sprites
Each supports different animation states, such as walking, jumping, etc.
*/

template<typename State>
struct Animation {
    std::unordered_map<State, std::vector<Sprite>> frames;
    float frameDuration;
    float currentTime;
    size_t currentFrame;
    State currentState;

    Animation(State s) : frameDuration(0.1f), currentTime(0.0f), currentFrame(0), currentState(s) {}

    // add a new animation state
    void addState(State newState, const std::vector<Sprite>&& newSprites) {
        frames[newState] = std::move(newSprites);
    }

    // get the next frame for the current state
    void next(float deltaTime) {
        currentTime += deltaTime;
        if (currentTime >= frameDuration) {
            currentFrame = (currentFrame + 1) % frames[currentState].size();
            currentTime = 0.0f;
        }
    }

    void setState(State newState) {
        if (frames.find(newState) != frames.end()) {
            currentState = newState;
            currentFrame = 0;
            currentTime = 0.0f;
        }
    }

    bool isAnimationComplete() const {
        // Check if the current frame is the last one
        return currentFrame >= frames.at(currentState).size() - 1;
    }

    PlayerState getState() const {
        return currentState;
    }

    const Sprite& getCurrentFrame() const {
        return frames.at(currentState)[currentFrame];
    }
};

struct Motion {
    vec2 position;
    float angle = 0;
    vec2 velocity = { 0, 0 };
    vec2 scale = { 0, 0 };
    vec2 acceleration = { 0, 0 };
    vec2 spawn_position = position;
};

struct TransformComponent {
    glm::vec3 position;
    glm::vec3 scale;
    float rotation;

    TransformComponent& operator=(const Motion& m) {
        position.x = m.position.x;
        position.y = m.position.y;
        position.z = 0;
        scale.x = m.scale.x;
        scale.y = m.scale.y;
        scale.z = 1.f;
        rotation = m.angle;
        return *this;
    }
};

// Player component
struct Player{
    bool attacking = false;
};

// Health component
struct Health
{
    int max_health;
    int current_health;
};

struct Damage
{
    int damage_dealt;
};

struct RecentlyDamagedTimer
{
    float counter_ms = 0.3f;
};

// A timer that will be associated to when the player can get damaged again 
struct InvincibilityTimer
{
    float counter_ms = 2000;
};

struct ProjectileTimer
{
    float max_time;
    float elapsed_time;
};

struct Projectile
{

};

struct HealthFlask
{
    int num_uses = 3;
};

struct Weapon
{
    
};


// anything that is hostile to the player
struct Hostile
{

};

// struct for attacking
struct Combat
{
    int max_frames = 240;
    int frames = max_frames;
};

// A timer that will be associated to dying salmon
struct DeathTimer
{
    float counter_ms = 3000;
};

struct Collision
{
    // Note, the first object is stored in the ECS container.entities
    Entity other; // the second object involved in the collision
    vec2 direction;
    vec2 overlap;
    Collision(Entity& other, const vec2& direction, const vec2& overlap) : direction(direction), overlap(overlap)
    { this->other = other; };
};

struct ColoredVertex
{
    vec3 position;
    vec3 color;
};

// Single Vertex Buffer element for textured sprites (textured.vs.glsl)
struct TexturedVertex
{
    vec3 position;
    vec2 texcoord;
};

// Mesh data structure for storing vertex and index buffers
struct Mesh
{
    static bool loadFromOBJFile(std::string obj_path, std::vector<ColoredVertex>& out_vertices, std::vector<uint16_t>& out_vertex_indices, vec2& out_size);
    vec2 original_size = {1,1};
    std::vector<ColoredVertex> vertices;
    std::vector<uint16_t> vertex_indices;
};

struct PlayerMeshes {
    std::unordered_map<PlayerState, Mesh> stateMeshes;
};

// Sets the brightness of the screen
struct ScreenState
{
    float darken_screen_factor = -1;
};

enum class TEXTURE_ASSET_ID {
    PLAYER_IDLE = 0,                      // idle.png
    PLAYER_WALK_1 = PLAYER_IDLE + 1,      // walk_1.png
    PLAYER_WALK_2 = PLAYER_WALK_1 + 1,    // walk_2.png
    PLAYER_WALK_3 = PLAYER_WALK_2 + 1,    // walk_3.png
    PLAYER_WALK_4 = PLAYER_WALK_3 + 1,    // walk_4.png
    PLAYER_JUMP_1 = PLAYER_WALK_4 + 1,    // jump_1.png
    PLAYER_JUMP_2 = PLAYER_JUMP_1 + 1,    // jump_2.png
    PLAYER_JUMP_3 = PLAYER_JUMP_2 + 1,    // jump_3.png
    PLAYER_JUMP_4 = PLAYER_JUMP_3 + 1,    // jump_4.png
    PLAYER_ATTACK_1 = PLAYER_JUMP_4 + 1,  // attack_1.png
    PLAYER_ATTACK_2 = PLAYER_ATTACK_1 + 1,// attack_2.png
    PLAYER_ATTACK_3 = PLAYER_ATTACK_2 + 1,// attack_3.png
    PLAYER_ATTACK_4 = PLAYER_ATTACK_3 + 1,// attack_4.png
    PLAYER_ATTACK_5 = PLAYER_ATTACK_4 + 1,// attack_5.png
    GOOMBA_WALK_ATTACK = PLAYER_ATTACK_5 + 1,  // goomba_walk_attack.PNG
    GOOMBA_WALK_HIT = GOOMBA_WALK_ATTACK + 1,  // goomba_walk_hit.PNG
    GOOMBA_WALK_IDLE = GOOMBA_WALK_HIT + 1,    // goomba_walk_idle.PNG
    GOOMBA_WALK_NOTICE = GOOMBA_WALK_IDLE + 1, // goomba_walk_notice.PNG
    GOOMBA_DEAD = GOOMBA_WALK_NOTICE + 1,      // goomba_dead.PNG
    CEILING_FALL = GOOMBA_DEAD + 1,            // ceiling_fall.png
    CEILING_HIT = CEILING_FALL + 1,            // ceiling_hit.png
    CEILING_IDLE = CEILING_HIT + 1,            // ceiling_idle.png
    CEILING_SPIT = CEILING_IDLE + 1,           // ceiling_spit.png 
    SPLASH_SCREEN = CEILING_SPIT + 1,          // splash_screen.png
    DEMO_GROUND = SPLASH_SCREEN + 1,           // demo_ground.png
    DEMO_CEILING = DEMO_GROUND + 1,            // demo_ceiling.png
    HEART_3 = DEMO_CEILING + 1,                 // heart_3.png
    HEART_2 = HEART_3 + 1,                     // heart_2.png
    HEART_1 = HEART_2 + 1,                     // heart_1.png
    HEART_0 = HEART_1 + 1,                     // heart_0.png
    CESSPIT_BG = HEART_0 + 1,                  // cesspit_bg.png
    ENTRANCE_BG = CESSPIT_BG + 1,              // entrance_bg.png
    SPACESHIP = ENTRANCE_BG + 1,               // spaceship.png
    PIPES = SPACESHIP + 1,                     // pipes.png
    CESSPIT_BOSS_BG = PIPES + 1,               // cesspit_boss_bg.PNG
    TEXTURE_COUNT = CESSPIT_BOSS_BG + 1        // Count of all textures
};
const int texture_count = (int)TEXTURE_ASSET_ID::TEXTURE_COUNT;

enum class EFFECT_ASSET_ID {
    PLAYER_EFFECT = 0,
    EFFECT_COUNT = PLAYER_EFFECT + 1
};
const int effect_count = (int)EFFECT_ASSET_ID::EFFECT_COUNT;

enum class GEOMETRY_BUFFER_ID {
    PLAYER_IDLE_MESH = 0,
    PLAYER_WALK_MESH = PLAYER_IDLE_MESH + 1,
    PLAYER_JUMP_MESH = PLAYER_WALK_MESH + 1,
    PLAYER_ATTACK_MESH = PLAYER_JUMP_MESH + 1,
    SPRITE = PLAYER_ATTACK_MESH + 1,
    GEOMETRY_COUNT = SPRITE + 1
};
const int geometry_count = (int)GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;

struct RenderRequest {
    TEXTURE_ASSET_ID used_texture = TEXTURE_ASSET_ID::TEXTURE_COUNT;
    EFFECT_ASSET_ID used_effect = EFFECT_ASSET_ID::EFFECT_COUNT;
    GEOMETRY_BUFFER_ID used_geometry = GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;
};

struct Gravity {
    float acceleration = 9.8f * TPS;
};


struct Patrol_AI {
    int direction = -1;
    float patrolMinX = 0;
    float patrolMaxX = 0;
    bool movingRight = true;
    bool chasing = false;
    float dashStartX = 0.0f;
    bool landed = false;
};

struct BoundingBox {
    vec2 p1; // Top Left
    vec2 p2; // Bottom Left
    vec3 p3; // Bottom Right
    vec4 p4; // Top Right
    float width = 1;
    float height = 1;
};

struct Environment {
    bool door = false;
};

struct Ground {

};

// Components used for Maps and Rooms
// For performance, consider:
// - having different vectors for different types of components
namespace std {
    template<>
    struct hash<Entity> {
        std::size_t operator()(const Entity& entity) const noexcept {
            return hash<unsigned int>()(entity);
        }
    };
}
struct Room {
    std::set<Entity> entities;

    void insert(Entity entity) {
        if (!has(entity)) {
            entities.insert(entity);
        }
    }

    bool has(Entity entity) {
        return entities.count(entity) > 0;
    }
};

struct Connection {
    Entity door;
    /*vec2 doorLocation;
    vec2 doorScale;*/
    Entity nextRoom;
    vec2 nextSpawn;
};

struct ConnectionList {
    std::vector<Connection> doors;
};    

// font character structure
struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
    char character;
};
