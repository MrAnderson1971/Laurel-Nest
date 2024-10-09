#define GL3W_IMPLEMENTATION
#include <gl3w.h>

// stlib
#include <chrono>

#include "physics_system.hpp"
#include "render_system.hpp"
#include "world_system.hpp"

//#include "ecs.hpp"
//#include "components.hpp"

using Clock = std::chrono::high_resolution_clock;

int main()
{
//    RenderSystem renderSystem;
//
//    if (!renderSystem.initOpenGL(1920, 1080, "Game"))
//    {
//        std::cerr << "Failed to initialize RenderSystem." << std::endl;
//        return -1;
//    }

    RenderSystem renderSystem(std::make_unique<WorldSystem>());


//    Entity splashScreenEntity;
//
//    int splashWidth; int splashHeight;
//    GLuint splashTextureID = renderSystem.loadTexture("splash_screen.png", splashWidth, splashHeight);
//
//    TransformComponent splashTransform;
//    splashTransform.position = glm::vec3(1920.0f / 2.0f, 1080.0f / 2.0f, 0.0f);
//    splashTransform.scale = glm::vec3(splashWidth, splashHeight, 1.0f);
//    splashTransform.rotation = 0.0f;
//
//    splashScreenEntity.addComponent<TransformComponent>(std::move(splashTransform));
//
//    SpriteComponent splashSprite;
//    splashSprite.textureID = splashTextureID;
//    splashSprite.width = 1.0f;
//    splashSprite.height = 1.0f;
//
//    splashScreenEntity.addComponent<SpriteComponent>(std::move(splashSprite));

//    Entity splashScreenEntity;
//    TransformComponent splashTransform;
//    splashTransform.position = glm::vec3(1920.0f / 2.0f, 1080.0f / 2.0f, 0.0f);
//    splashTransform.scale = glm::vec3(splashWidth, splashHeight, 1.0f);
//    splashTransform.rotation = 0.0f;
//    splashScreenEntity.addComponent<TransformComponent>(std::move(splashTransform));
//    SpriteComponent splashSprite;
//    splashSprite.textureID = splashTextureID;
//    splashSprite.width = 1.0f;
//    splashSprite.height = 1.0f;
//    splashScreenEntity.addComponent<SpriteComponent>(std::move(splashSprite));
//    renderSystem.renderLoop();
//    return 0;


//    renderSystem.renderLoop();

//    return 0;

    // Global systems
    WorldSystem world;
    RenderSystem renderer;
    PhysicsSystem physics;

    // Initializing window
    GLFWwindow* window = world.create_window();
    if (!window) {
        // Time to read the error message
        printf("Press any key to exit");
        getchar();
        return EXIT_FAILURE;
    }

    // initialize the main systems
    renderer.init(window);
    world.init(&renderer);

    // variable timestep loop
    auto t = Clock::now();
    while (!world.is_over()) {
        // Processes system messages, if this wasn't present the window would become unresponsive
        glfwPollEvents();

        // Calculating elapsed times in milliseconds from the previous iteration
        auto now = Clock::now();
        float elapsed_ms =
                (float)(std::chrono::duration_cast<std::chrono::microseconds>(now - t)).count() / 1000;
        t = now;

        world.step(elapsed_ms);
        physics.step(elapsed_ms);
        world.handle_collisions();

        renderer.draw();
    }

    return EXIT_SUCCESS;
}
