#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;

out vec2 textureCoordinates;

uniform mat4 modelMatrix;

void main()
{
   gl_Position = modelMatrix * vec4(position, 1.0);
   textureCoordinates = textureCoords;
}