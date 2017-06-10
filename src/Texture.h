#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include <string>
#include "glwrapper.h"
#include "stb_image.h"

class Texture {

public:
    Texture();

    bool load(const std::string &imagePath);

    bool load(void *compressedData, GLuint length);

    void bind(GLenum textureUnit);

private:

    bool loadImage(void *data, int width, int height, int channels);

private:
    GLuint textureId;
};


#endif //OPENGL_TEXTURE_H
