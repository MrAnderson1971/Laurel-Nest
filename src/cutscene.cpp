#include "cutscene.hpp"
#include "render_system.hpp"
#include "world_system.hpp"
#include "world_init.hpp"

Cutscene::Cutscene() : frameCount(0), seconds_passed(0.f), hasLoaded(false) {
    std::array<std::future<Image>, LAST_ANIMATION_FRAME> images;
    std::atomic<int> count;
    for (int i = 0; i < LAST_ANIMATION_FRAME; i++) {
        images[i] = loadImageData("opening_animation/opening_" + std::to_string(i) + ".png", count);
    }

    for (int i = 0; i < LAST_ANIMATION_FRAME; i++) {
        frames[i] = bindTexture(images[i].get());
        drawLoadingScreen(count.load(), LAST_ANIMATION_FRAME);
    }
}

void Cutscene::on_key(int, int, int action, int) {
    if (action == GLFW_PRESS && !hasLoaded) { // press any button to skip
        hasLoaded = true;
        renderSystem.getGameStateManager()->changeState<WorldSystem>();
    }
}

void Cutscene::update(float deltaTime) {
    seconds_passed += deltaTime;
    if (seconds_passed > MILISECONDS_PER_FRAME) {
        seconds_passed = 0;
        if (++frameCount >= LAST_ANIMATION_FRAME && !hasLoaded) {
            hasLoaded = true;
            renderSystem.getGameStateManager()->changeState<WorldSystem>();
        }
    }
}

void Cutscene::render() {
    TransformComponent transform{ vec3(window_width_px / 2.f, window_height_px / 2.f, 0.f), vec3(window_width_px, window_height_px, 1.f), 0.f };
    renderSystem.drawEntity(frames[frameCount].get(), transform);
}

void Cutscene::on_mouse_click(int, int, const vec2&, int) {}
void Cutscene::on_mouse_move(const vec2&) {}
