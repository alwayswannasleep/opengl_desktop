#include "Texture.h"

Texture::Texture() : textureId(0) {}

bool Texture::load(const std::string &imagePath) {
    int width;
    int height;
    int channels;

    void *data = stbi_load(imagePath.c_str(), &width, &height, &channels, 0);
    return loadImage(data, width, height, channels);
}

bool Texture::load(void *compressedData, GLuint length) {
    int width;
    int height;
    int channels;

    void *data = stbi_load_from_memory(static_cast<stbi_uc *>(compressedData), length, &width, &height, &channels, 0);

    return loadImage(data, width, height, channels);
}

bool Texture::loadImage(void *data, int width, int height, int channels) {
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GLenum format = channels == 4 ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    return true;
}

void Texture::bind(GLenum textureUnit) {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureId);
}
