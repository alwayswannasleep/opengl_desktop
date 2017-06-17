#ifndef OPENGL_CUBE_H
#define OPENGL_CUBE_H

#include "Model.h"

class Cube : public Model {

public:
    Cube();

protected:
    void initialize(const char *path) override;
};


#endif //OPENGL_CUBE_H
