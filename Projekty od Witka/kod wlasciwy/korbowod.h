#ifndef KORBOWOD_H_INCLUDED
#define KORBOWOD_H_INCLUDED

#include "opengl.h"
#include <vector>
#include <iostream>
#include "constants.h"

using namespace glm;

namespace std {
    class Korbowod {
    public:
        void DrawSolid();
        void DrawSolid(GLuint *tex, int number);
    };
    extern Korbowod korbowod;
}

#endif // KORBOWOD_H_INCLUDED
