#include "pause_state.hpp"
#include "ecs_registry.hpp"

PauseState::PauseState(): timePassed(0) {}

PauseState::~PauseState() {
    cleanup();
}

inline void PauseState::lerp(float start, float end, float t) const {
    t = clamp(t, 0.0f, 1.0f);
    registry.transforms.get(pauseScreenEntity).position[1] = start * (1 - t) + end * t;
}

void PauseState::init() {
    Sprite pauseSprite = renderSystem.loadTexture("pause_screen.png");

    TransformComponent pauseTransform;

    pauseTransform.position = glm::vec3(renderSystem.getWindowWidth() / 2.0f, 0.0f, 0.0f);

    pauseTransform.scale = glm::vec3(pauseSprite.width, pauseSprite.height, 1.0f);
    pauseTransform.rotation = 0.0f;

    // splashScreenEntity.addComponent<TransformComponent>(std::move(pauseTransform));
    registry.transforms.emplace(pauseScreenEntity, pauseTransform);

    // splashScreenEntity.addComponent<Sprite>(std::move(pauseSprite));
    registry.sprites.emplace(pauseScreenEntity, pauseSprite);
}

void PauseState::update(float deltaTime) {
    timePassed += deltaTime * 2.0f;
    lerp(0, renderSystem.getWindowHeight() / 2.0f, timePassed);
}

void PauseState::cleanup() {
    registry.remove_all_components_of(pauseScreenEntity);
}

void PauseState::render() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (registry.sprites.has(pauseScreenEntity) &&
        registry.transforms.has(pauseScreenEntity))
    {
        // Retrieve the Sprite and TransformComponent using the registry
        auto& sprite = registry.sprites.get(pauseScreenEntity);
        auto& transform = registry.transforms.get(pauseScreenEntity);

        // Use the render system to draw the entity
        renderSystem.drawEntity(sprite, transform);
    }
}

void PauseState::on_key(int button, int scancode, int action, int mods) {
    (void)button; (void)scancode; (void)action; (void)mods;
    if (action == GLFW_RELEASE && button == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(renderSystem.getWindow(), GLFW_TRUE);
    }
}

void PauseState::on_mouse_move(const glm::vec2& position) {
    (void)position;
}

void PauseState::on_mouse_click(int button, int action, const glm::vec2& position, int mods) {
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
            renderSystem.getGameStateManager()->resumeState();
    }
}
