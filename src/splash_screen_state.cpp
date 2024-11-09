#include "splash_screen_state.hpp"
#include "components.hpp"
#include "world_system.hpp"
#include <iostream>

SplashScreenState::~SplashScreenState() {
    SplashScreenState::cleanup();
}

void SplashScreenState::init()
{
    Sprite splashSprite = renderSystem.loadTexture("splash_screen.png");

    registry.transforms.emplace(splashScreenEntity, TransformComponent{
        vec3(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.0f - 50.f, 0.0f),
        vec3(splashSprite.width, splashSprite.height, 1.0f), 0.0
        });
    registry.sprites.emplace(splashScreenEntity, splashSprite);

    Sprite namesSprite = renderSystem.loadTexture("names.png");
    registry.transforms.emplace(namesEntity, TransformComponent{
        vec3(renderSystem.getWindowWidth() / 2.f, 50.f, 0.f),
        vec3(namesSprite.width / 2.f, namesSprite.height / 2.f, 1.f), 0.f
        });
    registry.sprites.emplace(namesEntity, namesSprite);

    MenuItem quitComponent(renderSystem.loadTexture("menu/quit_active.png"), renderSystem.loadTexture("menu/quit_inactive.png"),
        renderSystem.getWindowWidth() / 2.f, renderSystem.getWindowHeight() / 2.f + 200.f);
    registry.menuItems.emplace(quitEntity, quitComponent);
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
            renderSystem.getGameStateManager()->changeState<WorldSystem>();
        }
    }
}

void SplashScreenState::update(float )
{
}

void SplashScreenState::on_mouse_click(int button, int action, const glm::vec2& position, int) {
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
        if (registry.menuItems.get(quitEntity).isPointWithin(mouse_pos)) {
            renderSystem.closeWindow();
        }
    }
}

void SplashScreenState::render()
{
    // Clear the screen
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (const auto& entity : registry.sprites.entities) {
        if (!registry.transforms.has(entity)) {
            continue;
        }
        // Retrieve the Sprite and TransformComponent using the registry
        auto& sprite = registry.sprites.get(entity);
        auto& transform = registry.transforms.get(entity);

        // Use the render system to draw the entity
        renderSystem.drawEntity(sprite, transform);
    }

    for (const auto& component : registry.menuItems.components) {
        renderSystem.drawEntity(component.isPointWithin(mouse_pos) ? component.active : component.inactive,
            component.isPointWithin(mouse_pos) ? component.transformActive : component.transformInactive);
    }
}

void SplashScreenState::cleanup() {
    registry.clear_all_components();
}
