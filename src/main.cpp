#include <iostream>
#include "render_system.hpp"

int main()
{
    RenderSystem renderSystem;

    // Initialize OpenGL and create window
    if (!renderSystem.initOpenGL(1920, 1080, "Game"))
    {
        std::cerr << "Failed to initialize RenderSystem." << std::endl;
        return -1;
    }

    renderSystem.renderLoop();

    return 0;
}
