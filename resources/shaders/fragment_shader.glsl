#version 330 core

uniform sampler2D hdrSampler;

in vec2 textureCoordinates;
out vec4 color;

void main() {
    const float gamma = 2.2;
    vec3 hdrColor = texture(hdrSampler, textureCoordinates).rgb;
    vec3 mapped = hdrColor / (hdrColor + vec3(1.0));

    mapped = pow(mapped, vec3(1.0 / gamma));

    color = vec4(mapped, 1.0);
}