#define GL3W_IMPLEMENTATION
#include "render_system.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

RenderSystem::RenderSystem()
    : window(nullptr), shaderProgram(0), VAO(0), VBO(0), EBO(0), texture(0)
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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
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

    glViewport(0, 0, width, height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    loadShaders();

    setupVertices();

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
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(textures_path("splash_screen.png").c_str(), &width, &height, &nrChannels, 0);

    float imageWidth = static_cast<float>(width);
    float imageHeight = static_cast<float>(height);
    float windowWidth = static_cast<float>(this->windowWidth);
    float windowHeight = static_cast<float>(this->windowHeight);

    // Calculate the positions to center the image
    float left = (windowWidth - imageWidth) / 2.0f;
    float right = left + imageWidth;
    float bottom = (windowHeight - imageHeight) / 2.0f;
    float top = bottom + imageHeight;

    // Vertex data
    float vertices[] = {
        right, top, 0.0f,    1.0f, 1.0f,
        right, bottom, 0.0f, 1.0f, 0.0f,
        left, bottom, 0.0f,  0.0f, 0.0f,
        left, top, 0.0f,     0.0f, 1.0f 
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

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    projection = glm::ortho(0.0f, (float)windowWidth, 0.0f, (float)windowHeight);

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

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Error: Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glBindVertexArray(0);
}

void RenderSystem::renderLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        drawSplashScreen();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void RenderSystem::drawSplashScreen()
{
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void RenderSystem::cleanup()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    glDeleteTextures(1, &texture);

    glfwDestroyWindow(window);
    glfwTerminate();
}
