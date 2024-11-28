#include "cutscene.hpp"
#include "render_system.hpp"
#include "world_system.hpp"
#include "world_init.hpp"

OpeningCutscene::OpeningCutscene() : frameCount(0), seconds_passed(0.f), hasLoaded(false) {
    std::array<std::future<Image>, LAST_OPENING_ANIMATION_FRAME> images;
    std::atomic<int> count;
    for (int i = 0; i < LAST_OPENING_ANIMATION_FRAME; i++) {
        images[i] = loadImageData("opening_animation/opening_" + std::to_string(i) + ".png", count);
    }

    for (int i = 0; i < LAST_OPENING_ANIMATION_FRAME; i++) {
        frames[i] = bindTexture(images[i].get());
        drawLoadingScreen(count.load(), LAST_OPENING_ANIMATION_FRAME);
    }
}

void OpeningCutscene::on_key(int, int, int action, int) {
    if (action == GLFW_PRESS && !hasLoaded) { // press any button to skip
        hasLoaded = true;
        renderSystem.getGameStateManager()->changeState<WorldSystem>();
    }
}

void OpeningCutscene::update(float deltaTime) {
    seconds_passed += deltaTime;
    if (seconds_passed > SECONDS_PER_FRAME) {
        seconds_passed = 0;
        if (++frameCount >= LAST_OPENING_ANIMATION_FRAME && !hasLoaded) {
            hasLoaded = true;
            renderSystem.getGameStateManager()->changeState<WorldSystem>();
        }
    }
}

void OpeningCutscene::render() {
    TransformComponent transform{ vec3(window_width_px / 2.f, window_height_px / 2.f, 0.f), vec3(window_width_px, window_height_px, 1.f), 0.f };
    renderSystem.drawEntity(frames[frameCount].get(), transform);
}

void OpeningCutscene::on_mouse_click(int, int, const vec2&, int) {}
void OpeningCutscene::on_mouse_move(const vec2&) {}

PickupCutscene::PickupCutscene() : frameCount(0), seconds_passed(0.f), transitionFrame(-0.5f), finishedCutscene(false) {
    static bool hasLoaded = false;
    if (!hasLoaded) {
        hasLoaded = true;
        std::array<std::future<Image>, LAST_PICKUP_ANIMATION_FRAME> images;
        std::atomic<int> count;
        for (int i = 0; i < LAST_PICKUP_ANIMATION_FRAME; i++) {
            images[i] = loadImageData("pickup_animation/pick_up_" + std::to_string(i) + ".png", count);
        }

        for (int i = 0; i < LAST_PICKUP_ANIMATION_FRAME; i++) {
            frames[i] = bindTexture(images[i].get());
        }
    }
}

void PickupCutscene::update(float deltaTime) {
    if (!finishedCutscene) {
        seconds_passed += deltaTime;
        if (seconds_passed > SECONDS_PER_FRAME) {
            seconds_passed = 0;
            if (++frameCount >= LAST_PICKUP_ANIMATION_FRAME) {
                finishedCutscene = true;
                renderSystem.captureScreen();
            }
        }
    }
    else {
        while (transitionFrame < 1.f) {
            transitionFrame += deltaTime * 2.f;
            renderSystem.doGlassBreakTransition(clamp(static_cast<int>(transitionFrame * 100), 0, 100), 100);
        }
        renderSystem.getGameStateManager()->resumeState();
    }
}

void PickupCutscene::render() {
    if (!finishedCutscene) {
        TransformComponent transform{ vec3(window_width_px / 2.f, window_height_px / 2.f, 0.f), vec3(window_width_px, window_height_px, 1.f), 0.f };
        renderSystem.drawEntity(frames[frameCount].get(), transform);
    }
}
