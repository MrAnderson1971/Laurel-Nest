#include "splash_screen_state.hpp"
#include "components.hpp"
#include "world_system.hpp"
#include <iostream>

SplashScreenState::SplashScreenState()
{
}

SplashScreenState::~SplashScreenState() {
    cleanup();
}

void SplashScreenState::init()
{
    int splashWidth, splashHeight;
    GLuint splashTextureID = renderSystem.loadTexture("splash_screen.png", splashWidth, splashHeight);

    TransformComponent splashTransform;
    splashTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f, 0.0f);

    splashTransform.scale = glm::vec3(splashWidth, splashHeight, 1.0f);
    splashTransform.rotation = 0.0f;

    // splashScreenEntity.addComponent<TransformComponent>(std::move(splashTransform));
    registry.transforms.emplace(splashScreenEntity, std::move(splashTransform));

    Sprite splashSprite(splashTextureID);

    // splashScreenEntity.addComponent<Sprite>(std::move(splashSprite));
    registry.sprites.emplace(splashScreenEntity, std::move(splashSprite));
}

void SplashScreenState::on_key(int key, int, int action, int)
{
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_ESCAPE)
        {
            renderSystem.closeWindow();
        }
        else
        {
            // go to game
            renderSystem.getGameStateManager()->changeState(std::make_unique<WorldSystem>());
        }
    }
}

void SplashScreenState::on_mouse_move(const glm::vec2& ) {
}

void SplashScreenState::update(float )
{
}

void SplashScreenState::on_mouse_click(int button, int action, const glm::vec2& position, int) {
    if (action == GLFW_PRESS) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            std::cout << "left click at " << position[0] << ", " << position[1] << std::endl;
        }
    }
}


void SplashScreenState::render()
{
    // Clear the screen
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render the splash screen entity
//    auto& sprite = splashScreenEntity.getComponent<Sprite>();
//    auto& transform = splashScreenEntity.getComponent<TransformComponent>();
//    renderSystem.drawEntity(sprite, transform);

    // Check if the splash screen entity has the required components before rendering
    if (registry.sprites.has(splashScreenEntity) &&
        registry.transforms.has(splashScreenEntity))
    {
        // Retrieve the Sprite and TransformComponent using the registry
        auto& sprite = registry.sprites.get(splashScreenEntity);
        auto& transform = registry.transforms.get(splashScreenEntity);

        // Use the render system to draw the entity
        renderSystem.drawEntity(sprite, transform);
    }
}

void SplashScreenState::cleanup()
{
    registry.sprites.remove(splashScreenEntity);
    registry.transforms.remove(splashScreenEntity);
}