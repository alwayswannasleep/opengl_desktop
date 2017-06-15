#version 330 core

uniform mat4 modelMatrix;
uniform bool hasBones;
uniform mat4[60] bonesMatrices;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTextureCoordinates;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec4 inBonesIndeces;
layout (location = 4) in vec4 inBonesWeights;

out vec2 textureCoordinates;

void main() {
    vec4 transformedPosition = vec4(0.0);
    mat4 finalModelMatrix = mat4(1.0);

    if (hasBones) {
        ivec4 intIndeces = ivec4(inBonesIndeces);
        mat4 bonesMatrix = bonesMatrices[intIndeces.x] * inBonesWeights.x;
        bonesMatrix += bonesMatrices[intIndeces.y] * inBonesWeights.y;
        bonesMatrix += bonesMatrices[intIndeces.z] * inBonesWeights.z;
        bonesMatrix += bonesMatrices[intIndeces.w] * inBonesWeights.w;

        finalModelMatrix = modelMatrix * bonesMatrix;
        transformedPosition = finalModelMatrix * vec4(inPosition, 1.0);
    } else {
        finalModelMatrix = modelMatrix;
        transformedPosition = finalModelMatrix * vec4(inPosition, 1.0);
    }

    textureCoordinates = inTextureCoordinates;
    gl_Position = transformedPosition;
}