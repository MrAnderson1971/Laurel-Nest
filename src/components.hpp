#pragma once
#include "common.hpp"
#define STB_IMAGE_IMPLEMENTATION

struct TransformComponent {
    glm::vec3 position;
    glm::vec3 scale;
    float rotation;
};

struct Sprite {
    GLuint textureID;
    float width;
    float height;
};

struct Motion {
    vec2 position;
    vec2 velocity;
    vec2 acceleration;
    float angle;
};

struct Gravity {

};

struct BreakableDoor {
    bool broken = false;
};

struct Flamethrower {

};

struct Player {

};
