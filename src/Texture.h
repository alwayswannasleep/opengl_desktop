#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include <string>
#include "glwrapper.h"
#include "IL/il.h"

class Texture {

public:
    Texture();

    bool load(const std::string &imagePath);

    bool load(void *compressedData, ILuint length);

    void bind(GLenum textureUnit);

private:

    bool loadImage(ILuint imageId);

private:
    GLuint textureId;
};


#endif //OPENGL_TEXTURE_H
