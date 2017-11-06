#version 330 core

uniform sampler2D uTexture;
uniform vec3 cameraPosition;

in vec2 textureCoordinates;
in vec3 fragmentPosition;
in vec3 normal;

out vec4 color;

void main() {
    const float gamma = 2.2;
    vec3 lightColor = vec3(2.0);
    float ambientStrength = 0.1;
    float specularStrength = 0.1;
    vec3 lightPosition = vec3(100.0, 100.0, 35.0);
    vec3 lightDirection = normalize(lightPosition);
    vec3 viewDirection = normalize(cameraPosition - fragmentPosition);
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    vec4 objectColor = pow(texture(uTexture, textureCoordinates), vec4(gamma));

    vec3 ambientPart = ambientStrength * lightColor;

    float diffuseFactor = max(dot(normal, lightDirection), 0.0);
    vec3 diffusePart = diffuseFactor * lightColor;

    float specularFactor = pow(max(dot(normal, halfwayDirection), 0.0), 32.0);
    vec3 specularPart = specularStrength * specularFactor * lightColor;

    vec3 resultColor = (ambientPart + diffusePart + specularPart) * objectColor.rgb;
    color = vec4(resultColor, objectColor.a);
}
