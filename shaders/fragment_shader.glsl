#version 330 core

uniform sampler2D sampler;

in vec2 textureCoordinates;
out vec4 color;

void main() {
    color = vec4(0.8, 0.8, 0.8, 1.0);
}