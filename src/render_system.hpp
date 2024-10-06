#pragma once

#include <string>
#include "common.hpp"

class RenderSystem
{
public:
    RenderSystem();
    ~RenderSystem();

    bool initOpenGL(int width, int height, const std::string& title);
    void renderLoop();
    void cleanup();

private:
    void loadShaders();
    void setupVertices();
    void drawSplashScreen();
    std::string readShaderFile(const std::string& filePath);

    GLFWwindow* window;
    GLuint shaderProgram;
    GLuint VAO, VBO, EBO;
    GLuint texture;
    glm::mat4 projection;
    GLuint projectionLoc;

    int windowWidth;
    int windowHeight;
};
