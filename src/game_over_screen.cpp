#include "game_over_screen.hpp"
#include "ecs_registry.hpp"
#include "game_state_manager.hpp"
#include "splash_screen_state.hpp"

GameOverScreen::~GameOverScreen() {
    cleanup();
}

void GameOverScreen::init()
{
    int splashWidth, splashHeight;
    GLuint splashTextureID = renderSystem.loadTexture("game_over.png", splashWidth, splashHeight);

    TransformComponent splashTransform;
    splashTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f, 0.0f);

    splashTransform.scale = glm::vec3(splashWidth, splashHeight, 1.0f);
    splashTransform.rotation = 0.0f;

    registry.transforms.emplace(gameOverEntity, std::move(splashTransform));

    Sprite splashSprite(splashTextureID);

    registry.sprites.emplace(gameOverEntity, std::move(splashSprite));
}

void GameOverScreen::cleanup() {
    registry.remove_all_components_of(gameOverEntity);
}

void GameOverScreen::on_key(int, int, int, int) {}

void GameOverScreen::on_mouse_move(const vec2&) {}

void GameOverScreen::on_mouse_click(int button, int action, const vec2&, int) {
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
        renderSystem.getGameStateManager()->changeState(std::make_unique<SplashScreenState>());
    }
}

void GameOverScreen::render()
{
    // Clear the screen
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (registry.sprites.has(gameOverEntity) &&
        registry.transforms.has(gameOverEntity))
    {
        // Retrieve the Sprite and TransformComponent using the registry
        auto& sprite = registry.sprites.get(gameOverEntity);
        auto& transform = registry.transforms.get(gameOverEntity);

        // Use the render system to draw the entity
        renderSystem.drawEntity(sprite, transform);
    }
}

void GameOverScreen::update(float) {

}
