#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// Texture sampler
uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord);
}
