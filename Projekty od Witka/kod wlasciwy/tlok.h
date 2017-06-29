#ifndef TLOK_H_INCLUDED
#define TLOK_H_INCLUDED

#include "opengl.h"
#include <vector>
#include <iostream>
#include "constants.h"

using namespace glm;

namespace std {

    class Tlok {
    public:
        void DrawSolid();
        void DrawSolid(GLuint *tex, int number);
    };
    extern Tlok tlok;
}

#endif // TLOK_H_INCLUDED
