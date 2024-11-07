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
    Sprite splashSprite = renderSystem.loadTexture("splash_screen.png");

    TransformComponent splashTransform;
    splashTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f, 0.0f);

    splashTransform.scale = glm::vec3(splashSprite.width, splashSprite.height, 1.0f);
    splashTransform.rotation = 0.0f;

    // splashScreenEntity.addComponent<TransformComponent>(std::move(splashTransform));
    registry.transforms.emplace(splashScreenEntity, std::move(splashTransform));

    // splashScreenEntity.addComponent<Sprite>(std::move(splashSprite));
    registry.sprites.emplace(splashScreenEntity, std::move(splashSprite));

    Sprite namesSprite = renderSystem.loadTexture("names.png");

    TransformComponent namesTransform;
    namesTransform.position = vec3(renderSystem.getWindowWidth() / 2.f, renderSystem.getWindowHeight() - 100.f, 0.f);
    namesTransform.scale = vec3(namesSprite.width / 2.f, namesSprite.height / 2.f, 1.f);
    namesTransform.rotation = 0.f;
    registry.transforms.emplace(namesEntity, namesTransform);
    registry.sprites.emplace(namesEntity, namesSprite);
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
    if (registry.sprites.has(namesEntity) &&
        registry.transforms.has(namesEntity))
    {
        // Retrieve the Sprite and TransformComponent using the registry
        auto& sprite = registry.sprites.get(namesEntity);
        auto& transform = registry.transforms.get(namesEntity);

        // Use the render system to draw the entity
        renderSystem.drawEntity(sprite, transform);
    }
}

void SplashScreenState::cleanup()
{
    registry.remove_all_components_of(splashScreenEntity);
}