#ifndef WAL_KORBOWY_H_INCLUDED
#define WAL_KORBOWY_H_INCLUDED

#include "opengl.h"
#include <vector>
#include "constants.h"

using namespace glm;

namespace std {

    class Wal_Korbowy {
    public:
        void DrawSolid();
        void DrawSolid(vec3 color);
    };
    extern Wal_Korbowy wal_korbowy;
}

#endif // WAL_KORBOWY_H_INCLUDED
