#version 330 core

uniform mat4 modelMatrix;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTextureCoordinates;
layout (location = 2) in vec3 inNormal;

out vec2 textureCoordinates;

void main() {
    textureCoordinates = inTextureCoordinates;
    gl_Position = modelMatrix * vec4(inPosition, 1.0);
}