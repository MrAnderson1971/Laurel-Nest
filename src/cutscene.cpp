#include "cutscene.hpp"
#include "render_system.hpp"
#include "world_system.hpp"

Cutscene::Cutscene() : frame(0.f), hasLoaded(false) {
    std::array<std::future<Image>, LAST_ANIMATION_FRAME> images;
    std::atomic<int> count;
    for (int i = 0; i < LAST_ANIMATION_FRAME; i++) {
        images[i] = loadImageData("opening_animation/opening_" + std::to_string(i) + ".png", count);
    }

    for (int i = 0; i < LAST_ANIMATION_FRAME; i++) {
        frames[i] = bindTexture(images[i].get());
    }
}

void Cutscene::on_key(int, int, int action, int) {
    if (action == GLFW_PRESS && !hasLoaded) {
        hasLoaded = true;
        renderSystem.getGameStateManager()->changeState<WorldSystem>();
    }
}

void Cutscene::update(float deltaTime) {
    frame += deltaTime;
}

void Cutscene::render() {
    TransformComponent transform{ vec3(window_width_px / 2.f, window_height_px / 2.f, 0.f), vec3(window_width_px, window_height_px, 1.f), 0.f };
    renderSystem.drawEntity(frames[0].get(), transform);
}

void Cutscene::on_mouse_click(int, int, const vec2&, int) {}
void Cutscene::on_mouse_move(const vec2&) {}
