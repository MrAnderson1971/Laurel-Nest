#pragma once

#include <string>
#include "common.hpp"
#include "ecs.hpp"
#include "components.hpp"
#include "game_state_manager.hpp"
#include <array>	
#include "physics_system.hpp"
#include <map>				// map of character textures

class RenderSystem
{
    std::array<GLuint, texture_count> texture_gl_handles;
    std::array<ivec2, texture_count> texture_dimensions;

    const std::vector < std::pair<GEOMETRY_BUFFER_ID, std::string>> mesh_paths =
            {
                    std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::PLAYER_IDLE_MESH, mesh_path("mesh_walk_3.obj")),
                    std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::PLAYER_WALK_MESH, mesh_path("mesh_walk_3.obj")),
                    std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::PLAYER_JUMP_MESH, mesh_path("mesh_jump_3.obj")),
                    std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::PLAYER_ATTACK_MESH, mesh_path("mesh_attack_3.obj"))
            };

    // Make sure these paths remain in sync with the associated enumerators.

    std::array<GLuint, geometry_count> vertex_buffers;
    std::array<GLuint, geometry_count> index_buffers;
    std::array<Mesh, geometry_count> meshes;

public:

    static RenderSystem& instance() {
        static RenderSystem instance;
        return instance;
    }

    ~RenderSystem();

    RenderSystem(const RenderSystem&) = delete;
    RenderSystem& operator=(const RenderSystem&) = delete;

    void setGameStateManager(GameStateManager* gsm);
    GameStateManager* getGameStateManager() const;
    bool initOpenGL(int width, int height, const std::string& title);
    void renderLoop();
    void closeWindow();
    GLFWwindow* getWindow() const;
    int getWindowWidth() const;
    int getWindowHeight() const;
    void loadPlayerMeshes(Entity playerEntity);
    const Mesh& getPlayerMesh(Entity playerEntity, PlayerState state);

    void cleanup();
    Sprite loadTexture(const std::string& filePath);
    void drawEntity(const Sprite& sprite, const TransformComponent& transform);
    bool fontInit(const std::string& font_filename, unsigned int font_default_size);
    void renderText(std::string text, float x, float y, float scale, const glm::vec3& color, const glm::mat4& trans);

private:
    RenderSystem();

    void loadShaders();
    void setupVertices();
    std::string readShaderFile(const std::string& filePath);

    GLFWwindow* window;
    GLuint shaderProgram;
    GLuint VAO, VBO, EBO;
    glm::mat4 projection;
    GLuint projectionLoc;

    // font elements
    std::map<char, Character> m_ftCharacters;
    GLuint m_font_shaderProgram;
    GLuint m_font_VAO;
    GLuint m_font_VBO;

    int windowWidth;
    int windowHeight;

    // fps
    float fps;
    float frameTime;
    // FPS at 333 is kinda trash but on average, FPS is below 333, 
    // FPS at 334 can make it spike to 500, but on average is above 334
    const Uint32 maxFPS = 360;

    void calculateFPS();


    static void keyCallbackRedirect(GLFWwindow* wnd, int key, int scancode, int action, int mods);
    static void mouseMoveCallbackRedirect(GLFWwindow* wnd, double xpos, double ypos);
    static void mouseClickCallbackRedirect(GLFWwindow* wnd, int button, int action, int mods);

    GameStateManager* gameStateManager;
};

extern RenderSystem& renderSystem;
inline void renderMenuItem(const MenuItem& component, const vec2& mouse_pos) {
    renderSystem.drawEntity(component.isPointWithin(mouse_pos) ? component.active : component.inactive,
        component.isPointWithin(mouse_pos) ? component.transformActive : component.transformInactive);
}
