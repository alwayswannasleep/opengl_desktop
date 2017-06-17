#pragma once

#include "glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "assimp/matrix4x4.h"

namespace glm {
    inline mat4 toGlm(const aiMatrix4x4 &from) {
        mat4 to;

        to[0][0] = from.a1;
        to[0][1] = from.b1;
        to[0][2] = from.c1;
        to[0][3] = from.d1;
        to[1][0] = from.a2;
        to[1][1] = from.b2;
        to[1][2] = from.c2;
        to[1][3] = from.d2;
        to[2][0] = from.a3;
        to[2][1] = from.b3;
        to[2][2] = from.c3;
        to[2][3] = from.d3;
        to[3][0] = from.a4;
        to[3][1] = from.b4;
        to[3][2] = from.c4;
        to[3][3] = from.d4;

        return to;
    }

    inline vec2 toGlm(const aiVector2D &from) {
        vec2 to;

        to.x = from.x;
        to.y = from.y;

        return to;
    }

    inline vec3 toGlm(const aiVector3D &from) {
        vec3 to;

        to.x = from.x;
        to.y = from.y;
        to.z = from.z;

        return to;
    }

    inline vec3 toGlm(const aiColor3D &from) {
        vec3 to;

        to.x = from.r;
        to.y = from.g;
        to.z = from.b;

        return to;
    }

    inline quat toGlm(const aiQuaternion &from) {
        quat to;

        to.x = from.x;
        to.y = from.y;
        to.z = from.z;
        to.w = from.w;

        return to;
    }
}