#ifndef KULA_ZAPLONU_H_INCLUDED
#define KULA_ZAPLONU_H_INCLUDED

#include "opengl.h"
#include <vector>
#include <iostream>
#include "constants.h"

using namespace glm;

namespace std {
    class Kula_Zaplonu {
    public:
        void DrawSolid();
        void DrawSolid(vec3 color);
    };
    extern Kula_Zaplonu kula_zaplonu;
}

#endif // KULA_ZAPLONU_H_INCLUDED
