#include "wal_korbowy.h"
#include "wal_korbowy_data.h"



namespace std {

    Wal_Korbowy wal_korbowy;

void Wal_Korbowy::DrawSolid() {
    DrawSolid(vec3(0.8f, 0.8f, 0.8f));
}
void Wal_Korbowy::DrawSolid(vec3 color) {
     glEnableClientState(GL_VERTEX_ARRAY); //Podczas rysowania u�ywaj tablicy wierzcho�k�w
    glEnableClientState(GL_COLOR_ARRAY); //Podczas rysowania u�ywaj tablicy kolor�w
    glEnableClientState(GL_NORMAL_ARRAY);

    //vec3 color = vec3(0.8f, 0.8f, 0.8f);
    float wal_korbowyColors[wal_korbowyVertices * 3];
    for (int i = 0; i < wal_korbowyVertices; i++) {
        wal_korbowyColors[i*3] = color.x;
        wal_korbowyColors[i*3 + 1] = color.y;
        wal_korbowyColors[i*3 + 2] = color.z;
    }
    glVertexPointer(3,GL_FLOAT,0,wal_korbowyPositions); //Ustaw tablic� myCubeVertices jako tablic� wierzcho�k�w
    glColorPointer(3,GL_FLOAT,0,wal_korbowyColors); //Ustaw tablic� myCubeColors jako tablic� kolor�w
    glNormalPointer( GL_FLOAT, 0, wal_korbowyNormals);

    glDrawArrays(GL_TRIANGLES,0,wal_korbowyVertices); //Rysuj model

    //Posprz�taj po sobie
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}
}
