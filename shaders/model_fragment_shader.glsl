#version 330 core

uniform sampler2D uTexture;

in vec2 textureCoordinates;

out vec4 color;

void main() {
    color = texture(uTexture, textureCoordinates);
}
