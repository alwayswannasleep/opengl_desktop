#version 330 core

uniform mat4 modelMatrix;
uniform int hasBones;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTextureCoordinates;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in ivec4 inBonesIndeces;
layout (location = 4) in vec4 inBonesWeights;

out vec2 textureCoordinates;

void main() {
    textureCoordinates = inTextureCoordinates;
    gl_Position = modelMatrix * vec4(inPosition, 1.0);
}