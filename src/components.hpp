#pragma once
#include "common.hpp"
#define STB_IMAGE_IMPLEMENTATION

struct SpriteComponent {
    GLuint textureID;
    // Additional properties like texture coordinates, etc.

    SpriteComponent(GLuint texID)
        : textureID(texID) {}
};

struct TransformComponent {
    glm::vec2 position;
    glm::vec2 scale;
    float rotation; // Rotation in radians

    glm::mat4 mat;  // Transformation matrix

    // Constructor to initialize values
    TransformComponent(glm::vec2 pos = glm::vec2(0.0f), glm::vec2 scl = glm::vec2(1.0f), float rot = 0.0f)
        : position(pos), scale(scl), rotation(rot), mat(1.0f) {
        updateMatrix();
    }

    // Function to update the transformation matrix
    void updateMatrix() {
        mat = glm::mat4(1.0f);  // Initialize with identity matrix

        // First apply translation
        mat = glm::translate(mat, glm::vec3(position, 0.0f));

        // Then apply rotation
        mat = glm::rotate(mat, rotation, glm::vec3(0.0f, 0.0f, 1.0f));

        // Finally apply scaling
        mat = glm::scale(mat, glm::vec3(scale, 1.0f));
    }
};

struct FadeComponent {
    float alpha; // from 0.0 (transparent) to 1.0 (opaque)
    bool fadingIn; // true if fading in, false if fading out

    FadeComponent(float a, bool f) : alpha(a), fadingIn(f)
    {}
};

struct TimerComponent {
    float displayTime; // how long to display the splash screen
    float elapsedTime; // how much time has passed

    TimerComponent(float d, float e) : displayTime(d), elapsedTime(e)
    {}
};

enum class TEXTURE_ASSET_ID {
    SPLASH_SCREEN = 0,
    TEXTURE_COUNT = SPLASH_SCREEN + 1
};
const int texture_count = (int)TEXTURE_ASSET_ID::TEXTURE_COUNT;

enum class EFFECT_ASSET_ID {
    TEXTURED = 0,
    EFFECT_COUNT = TEXTURED + 1
};
const int effect_count = (int)EFFECT_ASSET_ID::EFFECT_COUNT;

enum class GEOMETRY_BUFFER_ID {
    SPRITE = 0,
    GEOMETRY_COUNT = SPRITE + 1
};
const int geometry_count = (int)GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;

struct TexturedVertex
{
    vec3 position;
    vec2 texcoord;
};

// Single Vertex Buffer element for non-textured meshes (coloured.vs.glsl & salmon.vs.glsl)
struct ColoredVertex
{
    vec3 position;
    vec3 color;
};

// Sets the brightness of the screen
struct ScreenState
{
    float darken_screen_factor = -1;
};
