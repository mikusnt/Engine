#include "wal_korbowy.h"
#include "wal_korbowy_data.h"

namespace std {
//Wal_Korbowy wal_korbow;

void Wal_Korbowy::DrawSolid() {
     glEnableClientState(GL_VERTEX_ARRAY); //Podczas rysowania u�ywaj tablicy wierzcho�k�w
    //glEnableClientState(GL_COLOR_ARRAY); //Podczas rysowania u�ywaj tablicy kolor�w

    glVertexPointer(3,GL_FLOAT,0,wal_korbowyPositions); //Ustaw tablic� myCubeVertices jako tablic� wierzcho�k�w
    //glColorPointer(3,GL_FLOAT,0,MSCubeColors); //Ustaw tablic� myCubeColors jako tablic� kolor�w

    glDrawArrays(GL_TRIANGLES,0,wal_korbowyVertices); //Rysuj model

    //Posprz�taj po sobie
    glDisableClientState(GL_VERTEX_ARRAY);
}
}
