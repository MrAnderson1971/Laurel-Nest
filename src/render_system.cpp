#define GL3W_IMPLEMENTATION
#include "render_system.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

RenderSystem::RenderSystem(WorldSystemPtr world)
    : window(nullptr), shaderProgram(0), VAO(0), VBO(0), EBO(0), worldSystem(std::move(world))
{
}

RenderSystem::~RenderSystem()
{
    cleanup();
}

bool RenderSystem::initOpenGL(int width, int height, const std::string& title)
{
    windowWidth = width;
    windowHeight = height;

    if (!glfwInit())
    {
        std::cerr << "Error: GLFW initialization failed" << std::endl;
        return false;
    }

    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(mode->width, mode->height, title.c_str(), primaryMonitor, nullptr);
    if (!window)
    {
        std::cerr << "Error: Window creation failed" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    if (gl3w_init())
    {
        std::cerr << "Error: gl3w initialization failed" << std::endl;
        glfwTerminate();
        return false;
    }

    glViewport(0, 0, mode->width, mode->height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    projection = glm::ortho(0.0f, static_cast<float>(mode->width), static_cast<float>(mode->height), 0.0f);

    loadShaders();

    setupVertices();

    worldSystem->setCloseWindowCallback([this]() { closeWindow(); });

    glfwSetKeyCallback(window, keyCallbackRedirect);
    glfwSetCursorPosCallback(window, mouseMoveCallbackRedirect);
    glfwSetWindowUserPointer(window, this);

    return true;
}

void RenderSystem::loadShaders()
{
    std::string vertexCode = readShaderFile(shader_path("textured.vs.glsl"));
    std::string fragmentCode = readShaderFile(shader_path("textured.fs.glsl"));

    const char* vertexShaderSource = vertexCode.c_str();
    const char* fragmentShaderSource = fragmentCode.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Error: Vertex Shader compilation failed\n" << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Error: Fragment Shader compilation failed\n" << infoLog << std::endl;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Error: Shader Program linking failed\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    projectionLoc = glGetUniformLocation(shaderProgram, "projection");
}

std::string RenderSystem::readShaderFile(const std::string& filePath)
{
    std::ifstream shaderFile(filePath);
    if (!shaderFile.is_open())
    {
        std::cerr << "Error: Failed to open shader file " << filePath << std::endl;
        return "";
    }
    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    return shaderStream.str();
}

void RenderSystem::setupVertices()
{
    float vertices[] = {
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f,
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f 
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3 
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void RenderSystem::closeWindow() {
    glfwSetWindowShouldClose(window, true);
}

void RenderSystem::renderLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        auto& spriteContainer = registry.get_component_container<Sprite>();
        for (size_t i = 0; i < spriteContainer.components.size(); ++i)
        {
            Entity entity = spriteContainer.entities[i];

            if (registry.get_component_container<TransformComponent>().has(entity))
            {
                Sprite& sprite = spriteContainer.components[i];
                TransformComponent& transform = registry.get_component_container<TransformComponent>().get(entity);

                drawEntity(sprite, transform);
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void RenderSystem::drawEntity(const Sprite& sprite, const TransformComponent& transform)
{
    glUseProgram(shaderProgram);

    // Create model matrix from transform component
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, transform.position);
    model = glm::rotate(model, glm::radians(transform.rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, transform.scale * glm::vec3(sprite.width, sprite.height, 1.0f));

    // Set uniforms
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Bind texture
    glBindTexture(GL_TEXTURE_2D, sprite.textureID);

    // Bind VAO and draw
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

GLuint RenderSystem::loadTexture(const std::string& filePath, int& outWidth, int& outHeight)
{
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(textures_path(filePath).c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format = GL_RGB;
        if (nrChannels == 1) {
            format = GL_RED;
        }
        else if (nrChannels == 3) {
            format = GL_RGB;
        }
        else if (nrChannels == 4) {
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        outWidth = width;
        outHeight = height;
    }
    else
    {
        std::cerr << "Failed to load texture at path: " << filePath << std::endl;
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
    stbi_image_free(data);

    return textureID;
}

void RenderSystem::cleanup()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
}

void RenderSystem::keyCallbackRedirect(GLFWwindow* wnd, int key, int scancode, int action, int mods) {
    RenderSystem* renderSystem = (RenderSystem*)glfwGetWindowUserPointer(wnd);
    if (renderSystem && renderSystem->worldSystem) {
        renderSystem->worldSystem->on_key(key, scancode, action, mods);  // Forward to WorldSystem
    }
}

void RenderSystem::mouseMoveCallbackRedirect(GLFWwindow* wnd, double xpos, double ypos) {
    RenderSystem* renderSystem = (RenderSystem*)glfwGetWindowUserPointer(wnd);
    if (renderSystem && renderSystem->worldSystem) {
        renderSystem->worldSystem->on_mouse_move(glm::vec2(xpos, ypos)); // Forward to WorldSystem
    }
}
