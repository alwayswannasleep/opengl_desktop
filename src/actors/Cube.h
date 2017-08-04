#ifndef OPENGL_CUBE_H
#define OPENGL_CUBE_H

#include "Model.h"

class Cube : public Model {

public:
    Cube();

    void render() override;

protected:
    void initialize(const char *path) override;

private:

    Texture *texture;
};


#endif //OPENGL_CUBE_H
