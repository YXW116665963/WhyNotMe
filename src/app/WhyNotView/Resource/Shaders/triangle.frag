#version 450 core

uniform vec4 objectColor;
uniform sampler2D ourTexture;

out vec4 FragColor;

in vec2 TexCoord;

void main() {
   FragColor = texture(ourTexture, TexCoord);
}