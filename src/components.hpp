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
