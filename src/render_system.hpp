#pragma once

#include <string>
#include "common.hpp"
#include "ecs.hpp"
#include "components.hpp"

class RenderSystem
{
public:
    RenderSystem();
    ~RenderSystem();

    bool initOpenGL(int width, int height, const std::string& title);
    void renderLoop();
    void cleanup();
    GLuint RenderSystem::loadTexture(const std::string& filePath, int& outWidth, int& outHeight);

private:
    void loadShaders();
    void setupVertices();
    void drawEntity(const SpriteComponent& sprite, const TransformComponent& transform);
    std::string readShaderFile(const std::string& filePath);

    GLFWwindow* window;
    GLuint shaderProgram;
    GLuint VAO, VBO, EBO;
    glm::mat4 projection;
    GLuint projectionLoc;

    int windowWidth;
    int windowHeight;
};
