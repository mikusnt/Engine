#ifndef ZAWOR_H_INCLUDED
#define ZAWOR_H_INCLUDED

#include "opengl.h"
#include <vector>
#include <iostream>
#include "constants.h"

using namespace glm;

namespace std {
    class Zawor {
    public:
        void DrawSolid();
        void DrawSolid(GLuint *tex, int number);
    };
    extern Zawor zawor;
}

#endif // ZAWOR_H_INCLUDED
