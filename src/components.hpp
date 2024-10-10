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

enum PlayerState {
    WALKING,
    ATTACKING,
    JUMPING,
};

template<typename State>
struct Animation {
    std::unordered_map<State, std::vector<Sprite>> frames;
    float frameDuration;
    float currentTime;
    size_t currentFrame;
    State currentState;

    Animation() : frameDuration(0.1f), currentTime(0.0f), currentFrame(0) {}

    void addState(State newState, const std::vector<Sprite>& newSprites) {
        frames[newState] = newSprites;
    }

    void next(float deltaTime) {
        currentTime += deltaTime;
        if (currentTime >= frameDuration) {
            currentFrame = (currentFrame + 1) % frames.size();
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

    const Sprite& getCurrentFrame() const {
        return frames.at(currentState)[currentFrame];
    }
};

struct Motion {
    vec2 position;
    vec2 velocity;
    vec2 acceleration;
    vec2 scale;
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

struct Mesh {

};
