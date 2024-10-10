#pragma once

#include <string>
#include "common.hpp"
#include "tiny_ecs.hpp"
#include "components.hpp"
#include "game_state_manager.hpp"

class RenderSystem
{
public:
    RenderSystem();
    ~RenderSystem();

    void setGameStateManager(GameStateManager* gsm);
    GameStateManager* getGameStateManager() const;
    bool initOpenGL(int width, int height, const std::string& title);
    void renderLoop();
    void closeWindow();
    GLFWwindow* getWindow() const;
    int getWindowWidth() const;
    int getWindowHeight() const;
    void cleanup();
    GLuint loadTexture(const std::string& filePath, int& outWidth, int& outHeight);
    void drawEntity(const Sprite& sprite, const TransformComponent& transform);

private:
    void loadShaders();
    void setupVertices();
    std::string readShaderFile(const std::string& filePath);

    GLFWwindow* window;
    GLuint shaderProgram;
    GLuint VAO, VBO, EBO;
    glm::mat4 projection;
    GLuint projectionLoc;

    int windowWidth;
    int windowHeight;

    static void keyCallbackRedirect(GLFWwindow* wnd, int key, int scancode, int action, int mods);
    static void mouseMoveCallbackRedirect(GLFWwindow* wnd, double xpos, double ypos);

    GameStateManager* gameStateManager;
};
