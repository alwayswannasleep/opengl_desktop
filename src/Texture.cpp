#include "Texture.h"

Texture::Texture() : textureId(0) {}

bool Texture::load(const std::string &imagePath) {
    auto id = ilGenImage();
    ilBindImage(id);

    if (!ilLoadImage(imagePath.c_str())) {
        return false;
    }

    return loadImage(id);
}

bool Texture::load(void *compressedData, ILuint length) {
    auto id = ilGenImage();
    ilBindImage(id);

    if (!ilLoadL(IL_PNG, compressedData, length)) {
        return false;
    }

    return loadImage(id);
}

bool Texture::loadImage(ILuint imageId) {
    if (!ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE)) {
        return false;
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(
            GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL),
            ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0,
            static_cast<GLenum>(ilGetInteger(IL_IMAGE_FORMAT)), GL_UNSIGNED_BYTE, ilGetData()
    );

    ilBindImage(0);
    ilDeleteImage(imageId);

    return true;
}

void Texture::bind(GLenum textureUnit) {
    glActiveTexture(textureUnit);
    glBindTexture(textureUnit, textureId);
}
