#version 330 core

layout(location = 0) in vec2 aPos; // Vertex position

out vec2 vUV; // Pass UV coordinates to fragment shader

void main()
{
    // Map vertex positions from [-1, 1] to clip space
    gl_Position = vec4(aPos, 0.0, 1.0);
    // Map positions to UV coordinates in [0, 1]
    vUV = (aPos + 1.0) * 0.5;
}
