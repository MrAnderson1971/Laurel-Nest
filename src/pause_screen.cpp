#include "pause_state.hpp"
#include "ecs_registry.hpp"
#include "splash_screen_state.hpp"
#include "options_menu.hpp"

void MenuState::on_key(int key, int, int action, int) {
    if (action == GLFW_RELEASE && key == GLFW_KEY_ESCAPE) {
        renderSystem.getGameStateManager()->resumeState();
    }
}

void MenuState::on_mouse_move(const vec2& position) {
    mouse_pos = position;
}

void MenuState::render() {
    // Clear the screen
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

PauseState::PauseState(): timePassed(0) {}

PauseState::~PauseState() {
    PauseState::cleanup();
}

inline void PauseState::lerp(float start, float end, float t) const {
    t = clamp(t, 0.0f, 1.0f);
    registry.transforms.get(pauseScreenEntity).position[1] = start * (1 - t) + end * t;
}

void PauseState::init() {
    Sprite pauseSprite = renderSystem.loadTexture("pause_screen.png");

    registry.transforms.emplace(pauseScreenEntity, TransformComponent{
        vec3(renderSystem.getWindowWidth() / 2.0f, renderSystem.getWindowHeight() / 2.f - 100, 0.0f),
        vec3(pauseSprite.width, pauseSprite.height, 1.0f), 0.f
    });
    registry.sprites.emplace(pauseScreenEntity, pauseSprite);

    MenuItem optionsComponent(renderSystem.loadTexture("menu/options_active.png"), renderSystem.loadTexture("menu/options_inactive.png"),
        renderSystem.getWindowWidth() / 2.f, renderSystem.getWindowHeight() / 2.f + 150.f);
    registry.menuItems.emplace(optionsEntity, optionsComponent);
    MenuItem quitComponent{renderSystem.loadTexture("menu/quit_active.png"), renderSystem.loadTexture("menu/quit_inactive.png"),
    renderSystem.getWindowWidth() / 2.f, renderSystem.getWindowHeight() / 2.f + 150.f + optionsComponent.transformInactive.scale.y * 3};
    registry.menuItems.emplace(quitEntity, quitComponent);
}

void PauseState::update(float deltaTime) {
    timePassed += deltaTime * 2.0f;
    lerp(0, renderSystem.getWindowHeight() / 2.0f, timePassed);
}

void PauseState::cleanup() {
    registry.remove_all_components_of(pauseScreenEntity);
    registry.remove_all_components_of(quitEntity);
    registry.remove_all_components_of(optionsEntity);
}

void PauseState::render() {
    MenuState::render();

    if (registry.sprites.has(pauseScreenEntity) &&
        registry.transforms.has(pauseScreenEntity))
    {
        // Retrieve the Sprite and TransformComponent using the registry
        auto& sprite = registry.sprites.get(pauseScreenEntity);
        auto& transform = registry.transforms.get(pauseScreenEntity);

        // Use the render system to draw the entity
        renderSystem.drawEntity(sprite, transform);
    }
    renderMenuItem(registry.menuItems.get(optionsEntity), mouse_pos);
    renderMenuItem(registry.menuItems.get(quitEntity), mouse_pos);
}

void PauseState::on_mouse_click(int button, int action, const glm::vec2& position, int mods) {
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
        if (registry.menuItems.get(optionsEntity).isPointWithin(mouse_pos)) {
            renderSystem.getGameStateManager()->pauseState<OptionsMenu>();
        }
        else if (registry.menuItems.get(quitEntity).isPointWithin(mouse_pos)) {
            renderSystem.getGameStateManager()->resetPausedStates<SplashScreenState>();
        }
    }
}
