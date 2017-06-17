#include "wal_korbowy.h"
#include "wal_korbowy_data.h"

namespace std {
//Wal_Korbowy wal_korbow;

void Wal_Korbowy::DrawSolid() {
     glEnableClientState(GL_VERTEX_ARRAY); //Podczas rysowania u¿ywaj tablicy wierzcho³ków
    //glEnableClientState(GL_COLOR_ARRAY); //Podczas rysowania u¿ywaj tablicy kolorów

    glVertexPointer(3,GL_FLOAT,0,wal_korbowyPositions); //Ustaw tablicê myCubeVertices jako tablicê wierzcho³ków
    //glColorPointer(3,GL_FLOAT,0,MSCubeColors); //Ustaw tablicê myCubeColors jako tablicê kolorów

    glDrawArrays(GL_TRIANGLES,0,wal_korbowyVertices); //Rysuj model

    //Posprz¹taj po sobie
    glDisableClientState(GL_VERTEX_ARRAY);
}
}
