#pragma once

#include <string>
#include "common.hpp"
#include "ecs.hpp"
#include "components.hpp"
#include "world_system.hpp"

class WorldSystem;

class RenderSystem
{
public:
    RenderSystem(WorldSystemPtr world);
    ~RenderSystem();

    bool initOpenGL(int width, int height, const std::string& title);
    void renderLoop();
    void cleanup();
    GLuint loadTexture(const std::string& filePath, int& outWidth, int& outHeight);

private:
    void loadShaders();
    void setupVertices();
    void drawEntity(const Sprite& sprite, const TransformComponent& transform);
    std::string readShaderFile(const std::string& filePath);

    GLFWwindow* window;
    GLuint shaderProgram;
    GLuint VAO, VBO, EBO;
    glm::mat4 projection;
    GLuint projectionLoc;
    WorldSystemPtr worldSystem;

    int windowWidth;
    int windowHeight;

    void closeWindow();
    static void keyCallbackRedirect(GLFWwindow* wnd, int key, int scancode, int action, int mods);
    static void mouseMoveCallbackRedirect(GLFWwindow* wnd, double xpos, double ypos);
};
